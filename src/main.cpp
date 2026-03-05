#include "configuration/configuration.h"
#include "net/channel_base.h"
#include "net/channel_udp.h"
#include "protocol/protocol_structure.h"
#include "protocol/tlv.h"

int main(int argc, char** argv)
{
    secmes::network::SocketUDP::udp_ch_init_dto_t udp_config {.ip = "127.0.0.1", .port = 1112};
    secmes::network::SocketUDP sock_udp(std::move(udp_config));
    sock_udp.init_base();


    /* SERVER */
    while(1) {
        auto ret = sock_udp.read_base();
        if (ret.has_value()) {
            for (const auto& b : *ret) {
                // Cast std::byte to char for character output
                std::cout << static_cast<char>(std::to_integer<int>(b));
            }
            std::cout << std::endl;
        } else {
            std::cout << "no value\n";
        }

    }

    /* CLIENT */
    // std::string str = "Hello, world!";
    // std::vector<std::byte> bytes(reinterpret_cast<const std::byte*>(str.data()),
    //                            reinterpret_cast<const std::byte*>(str.data() + str.size()));
    // secmes::network::SocketUDP::udp_ch_write_dto_t cfg {.send2ip = "127.0.0.1",.port=1112, .data=std::move(bytes)};
    // sock_udp.write_base(std::move(cfg));
    return 0;
}
