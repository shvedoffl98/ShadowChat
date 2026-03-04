#pragma once

#include <string>
#include <variant>
#include <concepts>
#include <type_traits>

#include <sys/socket.h>
#include <netinet/in.h>


namespace secmes
{

namespace network
{
/* Aliases */
using socket_fd_t = int32_t;
using port_type_t = uint16_t;
using ip_from_config_t = std::string;

template <typename CHANNEL_TYPE>
concept HasInit = requires(CHANNEL_TYPE t) {
    { t.init_impl() } -> std::same_as<bool>;
};

template <typename CHANNEL_TYPE>
concept HasClose = requires(CHANNEL_TYPE t) {
    { t.close_impl() } -> std::same_as<void>;
};

template <typename CHANNEL_TYPE>
concept HasRead = requires(CHANNEL_TYPE t) {
    { t.read_impl() } -> std::same_as<void>;
};

template <typename CHANNEL_TYPE>
concept HasWrite = requires(CHANNEL_TYPE t) {
    { t.write_impl() } -> std::same_as<void>;
};

template <typename CHANNEL_TYPE>
concept ChannelLike = HasInit<CHANNEL_TYPE>  &&
                      HasClose<CHANNEL_TYPE> &&
                      HasRead<CHANNEL_TYPE>  &&
                      HasWrite<CHANNEL_TYPE>;

template <typename ChannelDerived>
class ChannelBase
{
public:
    ChannelBase() = default;
    ~ChannelBase() = default;

public:
    bool init_base() requires ChannelLike<ChannelDerived>  { return derived().init_impl(); }
    void read_base() requires ChannelLike<ChannelDerived>  { return derived().read_impl(); }
    void write_base() requires ChannelLike<ChannelDerived> { return derived().write_impl(); }
    void close_base() requires ChannelLike<ChannelDerived> { return derived().close_impl(); }

private:
    ChannelDerived& derived()
    {
        return static_cast<ChannelDerived&>(*this);
    }
};

template <typename SocketType>
struct l3_channel_traits;

/* UDP/TCP,UNIX config */
struct l3_channel_config_t
{
    ip_from_config_t ip {};
    port_type_t port {};
};

/* DPDK config */
struct l2_channel_config_t
{
    /**/
};


template <typename CHANNEL_TYPE>
struct channel_config_t
{
    channel_config_t(l3_channel_config_t&& cfg)
    : config(std::move(cfg))
    {}

    channel_config_t(l2_channel_config_t&& cfg)
    : config(std::move(cfg))
    {}

    std::variant<l3_channel_config_t, /*dpdk*/ l2_channel_config_t> config {};
};

}
}
