#include "rays.hpp"
#include "bitboard.hpp"

BitBoard rays[N_DIRECTIONS][N_SQUARES];

namespace Rays
{
    template<Directions direction>
    BitBoard gen_diagonals_(Square square)
    {
        static_assert( direction == NORTH_WEST || direction == SOUTH_WEST || direction == SOUTH_EAST || direction == NORTH_EAST );
        return EMPTY_BB;
    }
    template<>
    BitBoard gen_diagonals_<NORTH_WEST>(Square square)
    {
        BitBoard north_west_BB = MAIN_ANTIDIAGONAL_BB;
        File file = get_file(square);
        File rank = get_rank(square) + 1;
        while (file != N_FILES) { north_west_BB = BitBoards::step<WEST>(north_west_BB); ++file; }
        while (rank != 0) { north_west_BB = BitBoards::step<NORTH>(north_west_BB); --rank; }

        return north_west_BB;
    }
    template<>
    BitBoard gen_diagonals_<SOUTH_WEST>(Square square)
    {
        BitBoard south_west_BB = MAIN_DIAGONAL_BB;
        File file = get_file(square);
        File rank = get_rank(square);
        while (file != N_FILES) { south_west_BB = BitBoards::step<WEST>(south_west_BB); ++file; }
        while (rank != N_RANKS) { south_west_BB = BitBoards::step<SOUTH>(south_west_BB); ++rank; }

        return south_west_BB;
    }
    template<>
    BitBoard gen_diagonals_<SOUTH_EAST>(Square square)
    {
        BitBoard south_east_BB = MAIN_ANTIDIAGONAL_BB;
        File file = get_file(square) + 1;
        File rank = get_rank(square);
        while (file != 0) { south_east_BB = BitBoards::step<EAST>(south_east_BB); --file; }
        while (rank != N_RANKS) { south_east_BB = BitBoards::step<SOUTH>(south_east_BB); ++rank; }

        return south_east_BB;
    }
    template<>
    BitBoard gen_diagonals_<NORTH_EAST>(Square square)
    {
        /* Much slower than looping through squares and calculating on fly, but it is precalculated so we should be fine */
        BitBoard north_east_BB = MAIN_DIAGONAL_BB;
        File file = get_file(square) + 1;
        File rank = get_rank(square) + 1;
        while (file != 0) { north_east_BB = BitBoards::step<EAST>(north_east_BB); --file; }
        while (rank != 0) { north_east_BB = BitBoards::step<NORTH>(north_east_BB); --rank; }

        return north_east_BB;
    }
    

    void init()
    {
        BitBoard north_BB = FILE_A_BB << 8;
        BitBoard south_BB = FILE_H_BB >> 8;

        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            rays[NORTH][square] = north_BB; north_BB <<= 1;

            rays[SOUTH][N_SQUARES - square - 1] = south_BB; south_BB >>= 1;

            rays[EAST][square] = ((1ULL << (square | 7)) - (1ULL << square)) << 1;

            rays[WEST][square] = (1ULL << square) - (1ULL << ((square & ~7)));

            rays[NORTH_WEST][square] = gen_diagonals_<NORTH_WEST>(square);

            rays[SOUTH_WEST][square] = gen_diagonals_<SOUTH_WEST>(square);

            rays[SOUTH_EAST][square] = gen_diagonals_<SOUTH_EAST>(square);

            rays[NORTH_EAST][square] = gen_diagonals_<NORTH_EAST>(square);
        }
    }

    BitBoard get(Direction direction, Square square)
    {
        return rays[direction][square];
    }
}