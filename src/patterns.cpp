#include "patterns.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "rays.hpp"

#include <iostream>

namespace Patterns::Magic
{
    struct Info
    {
        BitBoard* attack_table;
        BitBoard occupancy_mask;
        uint64_t magic;
        uint32_t bit_index;
    };

    Info bishop_info_table_[N_SQUARES];
    Info rook_info_table_[N_SQUARES];

    BitBoard gen_bishop_occupancy_mask_(Square square)
    {
        constexpr BitBoard rim_BB = FILE_A_BB | FILE_H_BB | RANK_1_BB | RANK_8_BB;

        BitBoard bishop_occupancy_mask =   (Rays::get(NORTH_WEST, square) | 
                                            Rays::get(SOUTH_WEST, square) | 
                                            Rays::get(SOUTH_EAST, square) | 
                                            Rays::get(NORTH_EAST, square)) &
                                            ~rim_BB;
        return bishop_occupancy_mask;
    }

    BitBoard gen_rook_occupancy_mask_(Square square)
    {
        BitBoard rook_occupancy_mask =   (Rays::get(NORTH, square) & ~RANK_8_BB) |
                                         (Rays::get(WEST,  square) & ~FILE_A_BB) |
                                         (Rays::get(SOUTH, square) & ~RANK_1_BB) |
                                         (Rays::get(EAST,  square) & ~FILE_H_BB) ;
        return rook_occupancy_mask;
    }

    void init_info_tables_()
    {
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            Magic::Info& bishop_info = bishop_info_table_[square];
            bishop_info.occupancy_mask = gen_bishop_occupancy_mask_(square);
            bishop_info.bit_index = popcount(bishop_info.occupancy_mask);

            Magic::Info& rook_info = rook_info_table_[square];
            rook_info.occupancy_mask = gen_rook_occupancy_mask_(square);
            rook_info.bit_index = popcount(rook_info.occupancy_mask);
        }
    }

    void init_()
    {
        init_info_tables_();
    }
}

namespace Patterns
{
    BitBoard knight_attacks_[N_SQUARES];
    BitBoard king_attacks_[N_SQUARES];

    BitBoard gen_knight_attacks_(Square square)
    {
        BitBoard knight_bitboard = BitBoards::square_set_in(square);
        BitBoard knight_attacks = EMPTY_BB;

        knight_attacks |= (knight_bitboard << 17) & (~FILE_A_BB)           ;
        knight_attacks |= (knight_bitboard << 10) & (~(FILE_A_BB|FILE_B_BB))  ;
        knight_attacks |= (knight_bitboard >>  6) & (~(FILE_A_BB|FILE_B_BB))  ;
        knight_attacks |= (knight_bitboard >> 15) & (~FILE_A_BB)           ;
        knight_attacks |= (knight_bitboard << 15) & (~FILE_H_BB)           ;
        knight_attacks |= (knight_bitboard <<  6) & (~(FILE_G_BB|FILE_H_BB))  ;
        knight_attacks |= (knight_bitboard >> 10) & (~(FILE_G_BB|FILE_H_BB))  ;
        knight_attacks |= (knight_bitboard >> 17) & (~FILE_H_BB)           ;

        return knight_attacks;
    }

    BitBoard gen_king_attacks_(Square square)
    {
        BitBoard king_bitboard = BitBoards::square_set_in(square);
        BitBoard king_attacks = EMPTY_BB;

        king_attacks |= BitBoards::step<NORTH>       (king_bitboard);
        king_attacks |= BitBoards::step<NORTH_WEST>  (king_bitboard);
        king_attacks |= BitBoards::step<WEST>        (king_bitboard);
        king_attacks |= BitBoards::step<SOUTH_WEST>  (king_bitboard);
        king_attacks |= BitBoards::step<SOUTH>       (king_bitboard);
        king_attacks |= BitBoards::step<SOUTH_EAST>  (king_bitboard);
        king_attacks |= BitBoards::step<EAST>        (king_bitboard);
        king_attacks |= BitBoards::step<NORTH_EAST>  (king_bitboard);

        return king_attacks;
    }



    void init_knight_attacks_()
    {
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            knight_attacks_[square] = gen_knight_attacks_(square);
        }
    }

    void init_king_attacks_()
    {
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            king_attacks_[square] = gen_king_attacks_(square);
        }
    }


    void init()
    {
        Magic::init_();
        
        init_knight_attacks_();
        init_king_attacks_();
    }

    BitBoard get_knight_attacks(Square square)
    {
        return knight_attacks_[square];
    }

    BitBoard get_king_attacks(Square square)
    {
        return king_attacks_[square];
    }
}