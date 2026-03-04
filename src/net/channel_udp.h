#pragma once

#include <cstring>
#include <utility>
#include <iostream>
#include <mutex>
#include <cstddef>

#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "channel_base.h"


namespace secmes
{

namespace network
{

class SocketUDP;

template<>
struct l3_channel_traits<SocketUDP>
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

class SocketUDP : public ChannelBase<SocketUDP>
{
/* Aliases */
private:
    using udp_socket_traits = l3_channel_traits<SocketUDP>;
    using sock_len_t = uint32_t;
    using epoll_fd_t = int32_t;

/* Constructors */
public:
    explicit SocketUDP(l3_channel_config_t&& cfg)
    : sock_fd(udp_socket_traits::sock_fd_not_inited),
      efd {},
      config(std::move(cfg)),
      r_mtx {},
      w_mtx {}
    {}

    SocketUDP() = delete;

/* Public API */
public:
    bool init_impl()
    {
        std::scoped_lock lock(r_mtx, w_mtx);
        bool ret_val {true};
        if (sock_fd == udp_socket_traits::sock_fd_not_inited) {
            struct sockaddr_in sock_addr {};
            sock_addr = {.sin_family = udp_socket_traits::domain_v4,
                        .sin_port = htons(config.port)};

            inet_aton(config.ip.c_str(), &sock_addr.sin_addr);
            sock_fd = socket(sock_addr.sin_family,
                            udp_socket_traits::type,
                            udp_socket_traits::protocol);

            fcntl(sock_fd, F_SETFL, O_NONBLOCK);

            if (!_bind_impl(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr))) {
                ret_val = false;
            } else {
                efd = epoll_create1(0);
                epoll_event ev {};
                ev.events = EPOLLIN;
                ev.data.fd = sock_fd;
                epoll_ctl(efd, EPOLL_CTL_ADD, sock_fd, &ev);
            }
        }
        return ret_val;
    }

    void read_impl()
    {
        std::lock_guard<std::mutex> lock(r_mtx);
        struct epoll_event events[1];
        int nfds;

        // unsigned char buffer[1200];
        while (1) {
            nfds = epoll_wait(efd, events, 1, -1);

            for (int i = 0; i < nfds; i++) {
                if (events[i].data.fd == sock_fd) {
                    // struct sockaddr_in addr;
                    // memset(&addr, 0, sizeof(sockaddr_in));
                    // socklen_t len = sizeof(addr);
                    // recvfrom(sock_fd, (unsigned char*)(buffer), 1200, MSG_WAITALL, (struct sockaddr*)&addr, &len);
                }
            }
        }
    }

    void write_impl()
    {
    }

    void close_impl()
    {
    }

/* Private methods */
private:
    uint32_t _bind_impl(socket_fd_t sock_fd,
                        struct sockaddr* sock_addr,
                        sock_len_t sock_len)
    {
        return bind(sock_fd, sock_addr, sock_len)
            != udp_socket_traits::sock_fd_not_inited;
    }

/* Owning obkects*/
private:
    socket_fd_t sock_fd;
    epoll_fd_t efd;
    l3_channel_config_t config;
    std::mutex r_mtx;
    std::mutex w_mtx;
};

}
}
