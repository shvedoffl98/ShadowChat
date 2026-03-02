#pragma once

#include <concepts>
#include <type_traits>
#include <iostream>


namespace secmes
{

namespace network
{

template <typename T>
concept SocketLike = requires(T t) {
    { t.bind_impl() } -> std::same_as<void>;
    { t.init_impl() } -> std::same_as<void>;
    { t.read_impl() } -> std::same_as<void>;
    { t.write_impl() } -> std::same_as<void>;
    { t.close_impl() } -> std::same_as<void>;
};

template <typename SocketDerived>
class SocketBase
{
public:
    SocketBase() = default;
    ~SocketBase() = default;

public:
    void bind() requires SocketLike<SocketDerived> { derived().bind_impl(); }
    void init() requires SocketLike<SocketDerived> { derived().init_impl(); }
    void read() requires SocketLike<SocketDerived> { derived().read_impl(); }
    void write() requires SocketLike<SocketDerived> { derived().write_impl(); }
    void close() requires SocketLike<SocketDerived> { derived().close_impl(); }

private:
    SocketDerived& derived()
    {
        return static_cast<SocketDerived&>(*this);
    }
};

class SocketUDP : public SocketBase<SocketUDP>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};


class SocketTCP : public SocketBase<SocketTCP>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};


class SocketUnix : public SocketBase<SocketUnix>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};


class SocketDPDK : public SocketBase<SocketDPDK>
{
public:
    void bind_impl() {}
    void init_impl() {}
    void read_impl() {}
    void write_impl() {}
    void close_impl() {}
};

}
}
