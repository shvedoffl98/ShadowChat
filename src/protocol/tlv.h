#pragma once

#include <cstdint>
#include <array>
#include <cstddef>
#include <vector>


namespace secmes
{

namespace protocol
{

enum class tlv_type_e : uint8_t
{
    PUBLIC_KEY = 0,
    TAG_HANDSHAKE,
    TEXT_UNIT
};

template <tlv_type_e TLV_TYPE>
class TLVHelper;


}
}