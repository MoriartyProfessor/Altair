#include "patterns.hpp"
#include "bitboard.hpp"

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