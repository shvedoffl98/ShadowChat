#include "configuration/configuration.h"
#include "net/channel_base.h"
#include "protocol/protocol.h"
#include "net/channel_udp.h"

int main(int argc, char** argv)
{
    secmes::network::l3_channel_config_t udp_config {.ip = "127.0.0.1", .port = 1256};
    secmes::network::SocketUDP sock_udp(std::move(udp_config));
    sock_udp.init_base();
    sock_udp.read_base();
    return 0;
}
