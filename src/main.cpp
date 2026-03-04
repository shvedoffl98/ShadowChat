#include "configuration/configuration.h"
#include "net/channel_base.h"
#include "protocol/protocol.h"
#include "net/channel_udp.h"

int main(int argc, char** argv)
{
    secmes::network::l3_channel_config_t udp_config {.ip = "127.0.0.1", .port = 1112};
    secmes::network::SocketUDP sock_udp(std::move(udp_config));
    sock_udp.init_base();


    /* FOR TEST UNCOMMENT IF SERVER */
    // while(1) {
    //     auto ret = sock_udp.read_base();
    //     if (ret.has_value()) {
    //         for (const auto& b : *ret) {
    //             // Cast std::byte to char for character output
    //             std::cout << static_cast<char>(std::to_integer<int>(b));
    //         }
    //         std::cout << std::endl;
    //     } else {
    //         std::cout << "no value\n";
    //     }

    // }

    /* FOR TEST DO COMMENT IF SERVER */
    std::string str = "Hello, world!";
    std::vector<std::byte> bytes(reinterpret_cast<const std::byte*>(str.data()),
                               reinterpret_cast<const std::byte*>(str.data() + str.size()));
    secmes::network::channel_write_data_udp_t cfg {.send2ip = "127.0.0.1",.port=1111, .data=std::move(bytes)};
    sock_udp.write_base(std::move(cfg));
    return 0;
}
