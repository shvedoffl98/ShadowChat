#pragma once

#include "builder.h"


namespace shadow
{

namespace builder
{

template <>
bool MessageBuilderProvider::_build_payload(shadow::protocol::message_t<shadow::protocol::message_type_e::CLIENT_HELLO> msg,
                                            /*OUT*/ std::vector<std::byte>&)
{
}

}
}
