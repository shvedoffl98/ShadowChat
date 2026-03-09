#pragma once


namespace utils
{

class UtilityProvider
{
public:
    /**
     * host to numeric byte order converter.
     */
    static uint64_t htonll(uint64_t num2convert)
    {
        static_assert(CHAR_BIT == 8);

        using ret_val_t = std::invoke_result_t<decltype(htonll), uint64_t>;
        ret_val_t ret_val {};

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        uint32_t high = num2convert >> 32;
        uint32_t low = num2convert & 0xFFFFFFFF;
        ret_val = static_cast<uint64_t>(low) << 32 | static_cast<uint64_t>(high);
#endif

        return ret_val;
    }
};

}
