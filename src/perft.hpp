#ifndef ALTAIR_PERFT_HPP
#define ALTAIR_PERFT_HPP

#include "position.hpp"

namespace Perft
{
    struct Result
    {
        uint64_t nodes = 0;
        uint64_t captures = 0;
        uint64_t en_passants = 0;
        uint64_t castles = 0;
        uint64_t promotions = 0;
        uint64_t checks = 0;
    };
    uint64_t test(Position position, uint32_t depth);
    void thorough_test(Position &position, Result &result, Move move, uint32_t depth);
}

#endif // ALTAIR_PERFT_HPP