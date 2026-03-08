#ifndef GWAIHIR_PATTERNS_HPP
#define GWAIHIR_PATTERNS_HPP

#include "defs.hpp"

namespace Patterns
{
    void init();
    BitBoard get_pawn_attacks(Square square, Color color);
    BitBoard get_knight_attacks(Square square);
    BitBoard get_bishop_attacks(Square square, BitBoard occupancy_bitboard);
    BitBoard get_rook_attacks(Square square, BitBoard occupancy_bitboard);
    BitBoard get_queen_attacks(Square square, BitBoard occupancy_bitboard);
    BitBoard get_king_attacks(Square square);
}

#endif // GWAIHIR_PATTERNS_HPP