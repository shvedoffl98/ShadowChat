#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>
#include "tlv.h"


namespace secmes
{

namespace protocol
{

enum class message_type_e : uint8_t
{
    CLIENT_HELLO = 0,
    SERVER_HELLO,
    HANDSHAKE_FINISH,
    DATA_UNIT
};

/* Constants */
constexpr uint16_t PACKET_SIZE_BYTES = 1200;

/* Protocol header */
struct __attribute__((packed)) protocol_header_t
{
    uint8_t magic {};
    uint8_t protocol_version {};
    uint8_t message_type {};
};

/* Message header */
struct __attribute__((packed)) message_header_t
{
    uint64_t session_id {};
    uint64_t packet_number {};
    uint64_t packet_count {};
    uint64_t seq_number {}; /* Used to protect data from replay attack */
    uint64_t timestamp {}; /* Used to protect data from replay attack */
    uint64_t nonce {}; /* Used to protect data from MITM. Makes same text to be cipherd differently */
};


/* Messages structures */
template <message_type_e msg_type>
struct __attribute__((packed)) message_payload_t;


template <>
struct __attribute__((packed)) message_payload_t<message_type_e::CLIENT_HELLO>
{
    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<std::byte, 32> client_public_key;
};


template <>
struct __attribute__((packed)) message_payload_t<message_type_e::SERVER_HELLO>
{
    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<std::byte, 32> server_public_key;
};


template <>
struct __attribute__((packed)) message_payload_t<message_type_e::HANDSHAKE_FINISH>
{
    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<std::byte, 16> hmac_tag;
};


template <>
struct message_payload_t<message_type_e::DATA_UNIT>
{
    static constexpr decltype(PACKET_SIZE_BYTES) DATA_UNIT_SIZE_BYTES =
        PACKET_SIZE_BYTES - sizeof(protocol_header_t) - sizeof(message_header_t);

    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<std::byte, DATA_UNIT_SIZE_BYTES> hmac_tag {};
};

}
}
