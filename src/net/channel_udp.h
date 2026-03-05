#pragma once

#include <cstring>
#include <utility>
#include <iostream>
#include <mutex>
#include <cstddef>

#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>

#include "channel_base.h"


namespace secmes
{

namespace network
{


class SocketUDP : public ChannelBase<SocketUDP>
{
public:
    /* UDP config */
    struct udp_ch_init_dto_t
    {
        ip_from_config_t ip {};
        port_type_t port {};
    };

    struct udp_ch_traits_t
    {
        using domain_t = int32_t;
        using protocol_t = int32_t;
        using type_t = int32_t;
        static constexpr domain_t domain_v4 = AF_INET;
        static constexpr domain_t domain_v6 = AF_INET6;
        static constexpr protocol_t protocol = IPPROTO_UDP;
        static constexpr type_t type = SOCK_DGRAM;
        static constexpr uint8_t sock_fd_not_inited = -1;
    };

    struct udp_ch_write_dto_t
    {
        ip_from_config_t send2ip {};
        port_type_t port {};
        std::vector<std::byte> data {};
    };

/* Aliases */
private:
    using sock_len_t = uint32_t;
    using epoll_fd_t = int32_t;

/* Constructors */
public:
    explicit SocketUDP(udp_ch_init_dto_t&& cfg)
    : all_closed {false},
      sock_fd(udp_ch_traits_t::sock_fd_not_inited),
      epfd {},
      config(std::move(cfg)),
      r_mtx {},
      w_mtx {}
    {}

    SocketUDP() = delete;
    ~SocketUDP() noexcept(true)
    {
        if (!all_closed) {
            _close_save();
        }
    }

/* Public API */
public:
    bool init_impl()
    {
        std::scoped_lock lock(r_mtx, w_mtx);
        bool ret_val {true};
        if (sock_fd == udp_ch_traits_t::sock_fd_not_inited) {
            struct sockaddr_in sock_addr {};
            sock_addr = {.sin_family = udp_ch_traits_t::domain_v4,
                        .sin_port = htons(config.port)};

            inet_aton(config.ip.c_str(), &sock_addr.sin_addr);
            sock_fd = socket(sock_addr.sin_family,
                            udp_ch_traits_t::type,
                            udp_ch_traits_t::protocol);

            fcntl(sock_fd, F_SETFL, O_NONBLOCK);

            if (!_bind_impl(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr))) {
                ret_val = false;
            } else {
                _do_modify_epoll(EPOLLIN, EPOLL_CTL_ADD);
            }
        }
        return ret_val;
    }

    std::optional<std::vector<std::byte>> read_impl()
    {
        /**
         * TODO: Make expandable to work with 1+ sockets.
         */

        std::lock_guard<std::mutex> lock(r_mtx);

        /* Return value */
        std::optional<std::vector<std::byte>> ret_val {};

        /* Just one socket fd */
        struct epoll_event events[1] {};
        int32_t socket_ready {};

        std::vector<std::byte> vec {};
        vec.resize(protocol::PACKET_SIZE_BYTES);
        socket_ready = epoll_wait(epfd, events, 1, -1);

        if (/*1*/socket_ready > 0 && events[0].data.fd == sock_fd) {
            struct sockaddr_in addr {};
            memset(&addr, 0, sizeof(sockaddr_in));
            sock_len_t len = sizeof(addr);
            ssize_t recv_bytes = recvfrom(events[0].data.fd,
                                    vec.data(),
                                    protocol::PACKET_SIZE_BYTES,
                                    0,
                                    (struct sockaddr*)&addr,
                                    &len);

            if (recv_bytes <= 0) {
                ret_val = std::nullopt;
            } else {
                vec.resize(recv_bytes);
                ret_val = std::move(vec);
            }
        }
        return ret_val;
    }

    bool write_impl(udp_ch_write_dto_t&& cfg)
    {
        bool ret_val {true};
        struct sockaddr_in sock_addr {};
        sock_addr = {.sin_family = udp_ch_traits_t::domain_v4,
                     .sin_port = htons(cfg.port)};

        inet_aton(cfg.send2ip.c_str(), &sock_addr.sin_addr);
        sock_fd = socket(sock_addr.sin_family,
                        udp_ch_traits_t::type,
                        udp_ch_traits_t::protocol);

        sock_len_t len = sizeof(sock_addr);
        if(sendto(sock_fd, cfg.data.data(), cfg.data.size(), 0, (struct sockaddr*)&sock_addr, len) < 0) {
            ret_val = false;
        }
        return ret_val;
    }

    void close_impl()
    {
        _close_save();
    }

/* Private methods */
private:
    uint32_t _bind_impl(socket_fd_t sock_fd,
                        struct sockaddr* sock_addr,
                        sock_len_t sock_len)
    {
        return bind(sock_fd, sock_addr, sock_len)
            != udp_ch_traits_t::sock_fd_not_inited;
    }

    bool _do_modify_epoll(EPOLL_EVENTS event, decltype(EPOLL_CTL_ADD) action)
    {
        /**
         * TODO: Modify code to process different actions
         */

        bool ret_val {true};
        if (epfd = epoll_create1(0); epfd < 0) {
            ret_val = false;
        } else {
            epoll_event ev {};
            ev.events = event;
            /* debug assert */
            assert(sock_fd > 0);
            ev.data.fd = sock_fd;
            if (epoll_ctl(epfd, action, sock_fd, &ev) < 0) {
                ret_val = false;
            }
        }
        return ret_val;
    }

    void _close_save()
    {
        /**
         * TODO: Think about error handling.
         */
        std::scoped_lock lock(r_mtx, w_mtx);
        if (!all_closed) {
            if (close(sock_fd) < 0) {
                /* log error */
                all_closed = false;
            }

            if (close(epfd) < 0) {
                /* log error */
            }
            all_closed = true;
        }
    }

/* Owning obkects*/
private:
    bool all_closed;
    socket_fd_t sock_fd;
    epoll_fd_t epfd;

    udp_ch_init_dto_t config;

    std::mutex r_mtx;
    std::mutex w_mtx;
};

}
}
