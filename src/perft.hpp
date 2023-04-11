#ifndef ALTAIR_PERFT_HPP
#define ALTAIR_PERFT_HPP

#include "defs.hpp"
#include "position.hpp"

namespace Perft
{
    uint64_t perft(const Position& position, uint32_t depth);
}

#endif // ALTAIR_PERFT_HPP