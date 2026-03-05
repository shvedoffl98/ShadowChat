#pragma once

#include <string>
#include <variant>
#include <concepts>
#include <type_traits>
#include <optional>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>

#include "../protocol/protocol_structure.h"


class SocketUDP;


namespace secmes
{

namespace network
{

/* Aliases */
using socket_fd_t = int32_t;
using port_type_t = uint16_t;
using ip_from_config_t = std::string;


/* Concepts (constraints) for CHANNEL CRTP fabric */
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
    { t.read_impl() } -> std::same_as<std::optional<std::vector<std::byte>>>;
};

template <typename CHANNEL_TYPE>
concept HasWrite = requires(CHANNEL_TYPE t, CHANNEL_TYPE::udp_ch_write_dto_t&& cfg) {
    { t.write_impl(std::move(cfg)) } -> std::same_as<bool>;
};

template <typename CHANNEL_TYPE>
concept ChannelLike = HasInit<CHANNEL_TYPE>  &&
                      HasClose<CHANNEL_TYPE> &&
                      HasRead<CHANNEL_TYPE>  &&
                      HasWrite<CHANNEL_TYPE>;

/* CRTP idiom */
template <typename ChannelDerived>
class ChannelBase
{
public:
    ChannelBase() = default;
    ~ChannelBase() = default;

public:
    bool init_base() requires ChannelLike<ChannelDerived>
    { return derived().init_impl(); }

    std::optional<std::vector<std::byte>> read_base()
        requires ChannelLike<ChannelDerived>
    { return derived().read_impl(); }

    template <typename T>
    bool write_base(T&& cfg)
        requires ChannelLike<ChannelDerived>
    { return derived().write_impl(std::move(cfg)); }

    void close_base()
        requires ChannelLike<ChannelDerived>
    { return derived().close_impl(); }

private:
    ChannelDerived& derived()
    {
        return static_cast<ChannelDerived&>(*this);
    }
};

}
}
