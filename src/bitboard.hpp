#ifndef ALTAIR_BITBOARD_HPP
#define ALTAIR_BITBOARD_HPP

#include "defs.hpp"

#include <string>

namespace BitBoards
{
    inline void set_square(BitBoard& bitboard, Square square)
    {
        bitboard |= (1ULL << square);
    }

    inline void clear_square(BitBoard& bitboard, Square square)
    {
        bitboard &= ~(1ULL << square);
    }

    inline bool is_square_set(BitBoard bitboard, Square square)
    {
        return (bitboard >> square) & 1;
    }

    constexpr BitBoard square_set_in(Square square)
    {
        return (1ULL << square);
    }

    constexpr BitBoard file_filled_in(File file)
    {
        constexpr BitBoard FILE_A_BB = 0x0101010101010101ULL;
        return FILE_A_BB << file;
    }

    constexpr BitBoard rank_filled_in(Rank rank)
    {
        constexpr BitBoard RANK_1_BB = 0xFFULL;
        return RANK_1_BB << (rank * 8);
    }

    std::string prettify(BitBoard);
    BitBoard from_string(const std::string& string);
}

/* Maybe should be moved inside the namespace */
constexpr BitBoard EMPTY_BB = 0x0ULL;
constexpr BitBoard UNIVERSE_BB = ~EMPTY_BB;

constexpr BitBoard LIGHT_SQUARES_BB = 0x55AA55AA55AA55AAULL;
constexpr BitBoard DARK_SQUARES_BB = ~LIGHT_SQUARES_BB;

constexpr BitBoard MAIN_DIAGONAL_BB = 0x8040201008040201ULL;
constexpr BitBoard MAIN_ANTIDIAGONAL_BB = 0x0102040810204080ULL;

constexpr BitBoard FILE_A_BB = BitBoards::file_filled_in(FILE_A);
constexpr BitBoard FILE_B_BB = BitBoards::file_filled_in(FILE_B);
constexpr BitBoard FILE_C_BB = BitBoards::file_filled_in(FILE_C);
constexpr BitBoard FILE_D_BB = BitBoards::file_filled_in(FILE_D);
constexpr BitBoard FILE_E_BB = BitBoards::file_filled_in(FILE_E);
constexpr BitBoard FILE_G_BB = BitBoards::file_filled_in(FILE_G);
constexpr BitBoard FILE_F_BB = BitBoards::file_filled_in(FILE_F);
constexpr BitBoard FILE_H_BB = BitBoards::file_filled_in(FILE_H);

constexpr BitBoard RANK_1_BB = BitBoards::rank_filled_in(RANK_1);
constexpr BitBoard RANK_2_BB = BitBoards::rank_filled_in(RANK_2);
constexpr BitBoard RANK_3_BB = BitBoards::rank_filled_in(RANK_3);
constexpr BitBoard RANK_4_BB = BitBoards::rank_filled_in(RANK_4);
constexpr BitBoard RANK_5_BB = BitBoards::rank_filled_in(RANK_5);
constexpr BitBoard RANK_6_BB = BitBoards::rank_filled_in(RANK_6);
constexpr BitBoard RANK_7_BB = BitBoards::rank_filled_in(RANK_7);
constexpr BitBoard RANK_8_BB = BitBoards::rank_filled_in(RANK_8);

namespace BitBoards
{
    template<Direction direction> inline BitBoard step(BitBoard bitboard) { static_assert(direction < N_DIRECTIONS); return N_SQUARES;}
    template<> inline BitBoard step<NORTH>      (BitBoard bitboard) {return (bitboard << 8);}
    template<> inline BitBoard step<NORTH_WEST> (BitBoard bitboard) {return (bitboard << 7) & (~FILE_H_BB);}
    template<> inline BitBoard step<WEST>       (BitBoard bitboard) {return (bitboard >> 1) & (~FILE_H_BB);}
    template<> inline BitBoard step<SOUTH_WEST> (BitBoard bitboard) {return (bitboard >> 9) & (~FILE_H_BB);}
    template<> inline BitBoard step<SOUTH>      (BitBoard bitboard) {return (bitboard >> 8);}
    template<> inline BitBoard step<SOUTH_EAST> (BitBoard bitboard) {return (bitboard >> 7) & (~FILE_A_BB);}
    template<> inline BitBoard step<EAST>       (BitBoard bitboard) {return (bitboard << 1) & (~FILE_A_BB);}
    template<> inline BitBoard step<NORTH_EAST> (BitBoard bitboard) {return (bitboard << 9) & (~FILE_A_BB);}
}

#endif // ALTAIR_BITBOARD_HPP