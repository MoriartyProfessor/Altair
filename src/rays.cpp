#include "rays.hpp"
#include "bitboard.hpp"

BitBoard rays[N_DIRECTIONS][N_SQUARES];

void Rays::init()
{
    BitBoard north_BB = FILE_A_BB;
    BitBoard south_BB = FILE_H_BB >> 8;
    BitBoard east_BB = RANK_1_BB;

    for(Square square = SQ_A1; square < N_SQUARES; ++square)
    {
        rays[NORTH][square] = north_BB; north_BB <<= 1;
        
        rays[SOUTH][N_SQUARES - square - 1] = south_BB; south_BB >>= 1;

        rays[EAST][square] = ((1ULL << (square | 7)) - (1ULL << square)) << 1;

        rays[WEST][N_SQUARES - square - 1] = (1ULL << square) - (1ULL << ((square & ~7)));
    }
}

BitBoard Rays::get(Direction direction, Square square)
{
    return rays[direction][square];
}