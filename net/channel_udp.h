#pragma once

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

/* Constructors */
public:
    explicit SocketUDP(l3_channel_config_t&& cfg)
    : config(std::move(cfg))
    {}

    SocketUDP() = delete;

/* Public API */
public:
    void init_impl()
    {
        struct sockaddr_in sock_addr {};
        sock_addr = {.sin_family = udp_socket_traits::domain_v4,
                     .sin_port = htons(config.port)};

        inet_aton(config.ip.c_str(), &sock_addr.sin_addr);
        sock_fd = socket(sock_addr.sin_family,
                         udp_socket_traits::type,
                         udp_socket_traits::protocol);

        if (bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr))
                == udp_socket_traits::sock_fd_not_inited) {
        }
    }

    void read_impl()
    {

    }

    void write_impl()
    {

    }

    void close_impl()
    {

    }

/* Private methods */
private:

/* Owning obkects*/
private:
    socket_fd_t sock_fd;
    l3_channel_config_t config;
};

}
}
