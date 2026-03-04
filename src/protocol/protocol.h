#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>


namespace secmes
{

namespace protocol
{

/* Constants */
constexpr uint16_t PACKET_SIZE_BYTES = 1200;

struct __attribute__((packed)) protocol_header_t
{
    uint8_t magic {};
    uint8_t protocol_version {};
    uint8_t message_type {};
};


struct __attribute__((packed)) message_header_t
{
    uint64_t sender_id {};
    uint64_t receiver_id {};
    uint64_t message_number {};
    uint64_t message_count {};

    /**
     * Signature. Should be calculated based on all
     * packet's fields except for signature
    */
    std::array<uint8_t, 64> signature {};
};


struct __attribute__((packed)) message_payload_t
{
    std::array<uint8_t, 60> cores {};
};

}
}
