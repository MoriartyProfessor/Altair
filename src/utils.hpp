#ifndef ALTAIR_UTILS_HPP
#define ALTAIR_UTILS_HPP

#include <cstdint>
#include <cmath>

inline uint32_t popcount(uint64_t x)
{
#if __GNUC__
    return __builtin_popcountll(x);
#elif _MSC_VER
    return __popcnt64(x);
#else
    /* Basic K&R implementation (chances are compiler is either in GNU dialect or is MSVSC) */
    uint32_t cnt = 0;
    for (; x != 0; x >>= 1)
        if (x & 1) cnt++;
    return cnt;
#endif
}

inline uint32_t LSB(uint64_t x)
{
#if __GNUC__
    return __builtin_ctzll(x);
#else
    return std::log2(x & -x);
#endif
}

inline uint32_t MSB(uint64_t x)
{
#if __GNUC__
    return 63 - __builtin_clzll(x);
#else
    /* Replace with something efficient in the future */
    uint32_t MSB;
    while (x >>= 1) {MSB++;}
    return MSB;
#endif
}

inline uint32_t pop_LSB(uint64_t& x)
{
#if __GNUC__
    uint32_t LSB = __builtin_ctzll(x);
#else
    uint32_t LSB = std::log2(x & -x);
#endif
    x &= ~(1ULL << LSB);
    return LSB;
}

#endif // ALTAIR_UTILS_HPP