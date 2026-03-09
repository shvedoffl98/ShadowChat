#include "configuration/configuration.h"
#include "net/channel_base.h"
#include "net/channel_udp.h"
#include "protocol/protocol_structure.h"
#include "protocol/tlv.h"
#include "crypto/crypto_provider.h"
#include "builder/builder.h"
#include <bitset>


int main(int argc, char** argv)
{
    shadow::network::SocketUDP::udp_ch_init_dto_t udp_config {.ip = "127.0.0.1", .port = 1111};
    shadow::network::SocketUDP sock_udp(std::move(udp_config));


    // /* SERVER */
    // while(1) {
    //     auto ret = sock_udp.read_base();
    //     if (ret.has_value()) {

    //         std::cout << ret->size() << std::endl;
    //         for (const auto& b : *ret) {
    //             if (b == *(ret->end()-2)) {
    //                 std::cout << htons(b);
    //             } else {
    //                 std::cout << b;
    //             }
    //         }
    //         std::cout << std::endl;
    //     } else {
    //         std::cout << "no value\n";
    //     }
    // }


    /* CLIENT */
    // std::cout << alignof(shadow::protocol::message_t<shadow::protocol::message_type_e::DATA_UNIT>) << std::endl;
    // std::cout << sizeof(shadow::protocol::message_t<shadow::protocol::message_type_e::DATA_UNIT>) << std::endl;

    // shadow::protocol::protocol_header_t prt {};
    // prt.message_type = static_cast<uint8_t>(shadow::protocol::message_type_e::CLIENT_HELLO);
    // prt.version = 2;

    // shadow::protocol::message_t<shadow::protocol::message_type_e::CLIENT_HELLO> msg {};
    // msg.prt_hdr = prt;
    // std::vector<uint8_t> vec {};
    // vec = MessageSerializer::build(std::move(msg));

    // std::vector<uint8_t> bytes(reinterpret_cast<const uint8_t*>(vec.data()),
    //                            reinterpret_cast<const uint8_t*>(vec.data() + vec.size()));
    // shadow::network::SocketUDP::udp_ch_write_dto_t cfg {.send2ip = "127.0.0.1",.port=1111, .data=std::move(bytes)};
    // sock_udp.write_base(std::move(cfg));

    uint64_t num = 123138719874918734;
    std::cout << "num: " << num << std::endl;
    std::bitset<64> b2(num);
    std::cout << "bitset: " << b2 << std::endl;
    uint64_t num_2 = utils::UtilityProvider::htonll(num);
    std::bitset<64> b3(num_2);
    std::cout << "num2: " << num_2 << std::endl;

    std::cout << "bitset: " << b3 << std::endl;

    uint64_t num_3 = utils::UtilityProvider::htonll(num_2);
    std::bitset<64> b4(num_3);
    std::cout << "num3: " << num_3 << std::endl;
    std::cout << "bitset: " << b4 << std::endl;
    // // for (const auto& x : vec) {
    // //     std::cout << (int)x;
    // // }
    // uint64_t* u66 = reinterpret_cast<uint64_t*>(vec.data());
    // // std::cout << "bitset: " << std::bitset<64>(*u66);

    // auto vec_2 = MessageSerializer::htonll(*u66);
    // uint64_t* u66_2 = reinterpret_cast<uint64_t*>(vec_2.data());

    // std::cout << "dec: " << *u66_2;
    // std::cout << std::endl;


    // auto [pub, priv] = crypto::CryptoProvider::get_pub_priv_keys<crypto::diffie_hellman_X25519_traits_t>();

    // for (const auto& x : pub) {
    //     std::cout << x << " ";
    // }
    // std::cout << std::endl;

    // auto ret = crypto::CryptoProvider::get_hash<crypto::hash_sha256_hmac_traits_t>((reinterpret_cast<const uint8_t*>(str.data())), strlen(str.data()));

    // for (const auto& x : ret) {
    //     std::cout << x << " ";
    // }
    // std::cout << std::endl;

    return 0;
}
