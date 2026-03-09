#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <vector>

#include "tlv.h"
#include "../crypto/crypto_provider.h"


namespace shadow
{

namespace protocol
{


enum class message_type_e : uint8_t
{
    CLIENT_HELLO = 1,
    SERVER_HELLO,
    HANDSHAKE_FINISH,
    DATA_UNIT
};

/* Constants */
constexpr uint16_t PACKET_SIZE_BYTES = 1200;

/* Protocol header */
struct __attribute__((packed)) protocol_header_t
{
    static constexpr std::array<uint8_t, 3> magic {'O', 'B', 'S'};
    uint16_t version {};
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


/**
 * Messages structures.
 * Should be all std::is_standard_layout<T> == true.
 */
template <message_type_e msg_type>
struct __attribute__((packed)) message_t;


template <>
struct __attribute__((packed)) message_t<message_type_e::CLIENT_HELLO>
{
    using diffie_hellman_t = crypto::diffie_hellman_X25519_traits_t;

    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<uint8_t, diffie_hellman_t::ret_size_b> client_public_key;
};


template <>
struct __attribute__((packed)) message_t<message_type_e::SERVER_HELLO>
{
    using diffie_hellman_t = crypto::diffie_hellman_X25519_traits_t;

    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<uint8_t, diffie_hellman_t::ret_size_b> server_public_key;
};


template <>
struct __attribute__((packed)) message_t<message_type_e::HANDSHAKE_FINISH>
{
    using hash_sha256_t = crypto::hash_sha256_hmac_traits_t;
    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<uint8_t, hash_sha256_t::ret_size_b> hmac_tag;
};


template <>
struct message_t<message_type_e::DATA_UNIT>
{
    static constexpr decltype(PACKET_SIZE_BYTES) DATA_UNIT_SIZE_BYTES =
        PACKET_SIZE_BYTES - sizeof(protocol_header_t) - sizeof(message_header_t);

    protocol_header_t prt_hdr {};
    message_header_t msg_hdr {};
    std::array<uint8_t, DATA_UNIT_SIZE_BYTES> hmac_tag {};
};

}
}
