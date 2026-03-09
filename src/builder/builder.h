#pragma once

#include <vector>
#include <type_traits>
#include <arpa/inet.h>
#include <climits>

#include "../protocol/protocol_structure.h"
#include "../utils/utils.h"


namespace shadow
{

namespace builder
{

class MessageSerializer
{
public:
    template <shadow::protocol::message_type_e MSG_TYPE>
    static std::vector<uint8_t> build(shadow::protocol::message_t<MSG_TYPE>&& msg)
    {
        using ret_val_t = std::invoke_result_t<decltype(build<MSG_TYPE>),
                                               shadow::protocol::message_t<MSG_TYPE>&&>;
        ret_val_t ret_val {};
        _build_protocol_header(msg.prt_hdr, /*OUT*/ret_val);
        _build_message_header(msg, /*OUT*/ret_val);
        // _build_payload(msg, /*OUT*/ret_val);
        return ret_val;
    }

    static bool _build_protocol_header(const shadow::protocol::protocol_header_t& prt_hdr,
                                       /*OUT*/ std::vector<uint8_t>& vec)
    {
        using ret_val_t = std::invoke_result_t<decltype(_build_protocol_header),
                                               shadow::protocol::protocol_header_t,
                                               std::vector<uint8_t>&>;
        ret_val_t ret_val {true};
        auto inserted = vec.insert(vec.end(), prt_hdr.magic.begin(), prt_hdr.magic.end());
        if (inserted == vec.end()) {
            ret_val = false;
        }

        auto version_converted = htons(prt_hdr.version);
        uint8_t* u8_ptr = reinterpret_cast<uint8_t*>(&version_converted);
        vec.insert(vec.end(), u8_ptr, u8_ptr + 2);

        vec.push_back(prt_hdr.message_type);
        return ret_val;
    }

    static void _build_message_header(shadow::protocol::message_header_t&& msg_hdr,
                                      /*OUT*/ std::vector<uint8_t>& vec)
    {
        vec.push_back(utils::UtilityProvider::htonll(msg_hdr.session_id));
        vec.push_back(utils::UtilityProvider::htonll(msg_hdr.packet_number));
        vec.push_back(utils::UtilityProvider::htonll(msg_hdr.seq_number));
        vec.push_back(utils::UtilityProvider::htonll(msg_hdr.packet_count));
        vec.push_back(utils::UtilityProvider::htonll(msg_hdr.timestamp));
        vec.push_back(utils::UtilityProvider::htonll(msg_hdr.nonce));
    }

    template <shadow::protocol::message_type_e MSG_TYPE>
    static bool _build_payload(shadow::protocol::message_t<MSG_TYPE> msg,
                               /*OUT*/ std::vector<uint8_t>&);
};

}
}
