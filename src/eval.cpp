#include "eval.hpp"
#include "utils.hpp"

namespace Evaluation
{
    constexpr int32_t MATERIAL_VALUES[N_PIECE_TYPES] = {100, 330, 350, 525, 1000, 20000};
    
    /* Currently using relative_square() function to access color specific PST value.
       Worth experimenting with separatge PSTs for each color*/
    
    constexpr int32_t PAWN_PST[N_SQUARES] = { 0,  0,  0,  0,  0,  0,  0,  0,
                                             50, 50, 50, 50, 50, 50, 50, 50,
                                             10, 10, 20, 30, 30, 20, 10, 10,
                                              5,  5, 10, 25, 25, 10,  5,  5,
                                              0,  0,  0, 20, 20,  0,  0,  0,
                                              5, -5,-10,  0,  0,-10, -5,  5,
                                              5, 10, 10,-20,-20, 10, 10,  5,
                                              0,  0,  0,  0,  0,  0,  0,  0};

    constexpr int32_t KNIGHT_PST[N_SQUARES] = { -50,-40,-30,-30,-30,-30,-40,-50,
                                                -40,-20,  0,  0,  0,  0,-20,-40,
                                                -30,  0, 10, 15, 15, 10,  0,-30,
                                                -30,  5, 15, 20, 20, 15,  5,-30,
                                                -30,  0, 15, 20, 20, 15,  0,-30,
                                                -30,  5, 10, 15, 15, 10,  5,-30,
                                                -40,-20,  0,  5,  5,  0,-20,-40,
                                                -50,-40,-30,-30,-30,-30,-40,-50};
                                                
    constexpr int32_t BISHOP_PST[N_SQUARES] = { -20,-10,-10,-10,-10,-10,-10,-20,
                                                -10,  0,  0,  0,  0,  0,  0,-10,
                                                -10,  0,  5, 10, 10,  5,  0,-10,
                                                -10,  5,  5, 10, 10,  5,  5,-10,
                                                -10,  0, 10, 10, 10, 10,  0,-10,
                                                -10, 10, 10, 10, 10, 10, 10,-10,
                                                -10,  5,  0,  0,  0,  0,  5,-10,
                                                -20,-10,-10,-10,-10,-10,-10,-20};

    constexpr int32_t ROOK_PST[N_SQUARES] = {  0,  0,  0,  0,  0,  0,  0,  0,
                                               5, 10, 10, 10, 10, 10, 10,  5,
                                              -5,  0,  0,  0,  0,  0,  0, -5,
                                              -5,  0,  0,  0,  0,  0,  0, -5,
                                              -5,  0,  0,  0,  0,  0,  0, -5,
                                              -5,  0,  0,  0,  0,  0,  0, -5,
                                              -5,  0,  0,  0,  0,  0,  0, -5,
                                               0,  0,  0,  5,  5,  0,  0,  0};

    constexpr int32_t QUEEN_PST[N_SQUARES] = {-20,-10,-10, -5, -5,-10,-10,-20,
                                              -10,  0,  0,  0,  0,  0,  0,-10,
                                              -10,  0,  5,  5,  5,  5,  0,-10,
                                               -5,  0,  5,  5,  5,  5,  0, -5,
                                                0,  0,  5,  5,  5,  5,  0, -5,
                                              -10,  5,  5,  5,  5,  5,  0,-10,
                                              -10,  0,  5,  0,  0,  0,  0,-10,
                                              -20,-10,-10, -5, -5,-10,-10,-20};

    constexpr int32_t KING_MG_PST[N_SQUARES] = {-30,-40,-40,-50,-50,-40,-40,-30,
                                                -30,-40,-40,-50,-50,-40,-40,-30,
                                                -30,-40,-40,-50,-50,-40,-40,-30,
                                                -30,-40,-40,-50,-50,-40,-40,-30,
                                                -20,-30,-30,-40,-40,-30,-30,-20,
                                                -10,-20,-20,-20,-20,-20,-20,-10,
                                                 20, 20,  0,  0,  0,  0, 20, 20,
                                                 20, 30, 10,  0,  0, 10, 30, 20};

    constexpr int32_t KING_EG_PST[N_SQUARES] = {-50,-40,-30,-20,-20,-30,-40,-50,
                                                -30,-20,-10,  0,  0,-10,-20,-30,
                                                -30,-10, 20, 30, 30, 20,-10,-30,
                                                -30,-10, 30, 40, 40, 30,-10,-30,
                                                -30,-10, 30, 40, 40, 30,-10,-30,
                                                -30,-10, 20, 30, 30, 20,-10,-30,
                                                -30,-30,  0,  0,  0,  0,-30,-30,
                                                -50,-30,-30,-30,-30,-30,-30,-50};

    const int32_t* PS_TABLES[N_PIECE_TYPES - 1] = {
        PAWN_PST, 
        KNIGHT_PST, 
        BISHOP_PST, 
        ROOK_PST, 
        QUEEN_PST
    };

    bool is_endgame(const Position &position) 
    {
        auto is_endgame_for_side = [&](Color color) {
            if (position.piece_count(make_piece(color, QUEEN)) == 0)
                return true;
            auto minor_piece_count = position.piece_count(make_piece(color, KNIGHT)) +
                                     position.piece_count(make_piece(color, BISHOP)) +
                                     position.piece_count(make_piece(color, ROOK));
            return minor_piece_count <= 1; 
        };
        return is_endgame_for_side(WHITE) && is_endgame_for_side(BLACK);
    }

    int32_t accumulate_pst(const Position& position, PieceType piece_type, Color color, const int32_t* pst) 
    {
        int sum = 0;
        auto piece_bitboard = position.piece_bitboard(color, piece_type);
        while(piece_bitboard) 
        {
            Square sq = pop_LSB(piece_bitboard);
            sum += pst[relative_square(sq, color)];
        }
        return sum;
    }

    int32_t evaluate(const Position &position)
    {
        int32_t eval = 0;
        for(PieceType piece_type = PAWN; piece_type != N_PIECE_TYPES; ++piece_type) 
            eval += MATERIAL_VALUES[piece_type] * 
                    (position.piece_count(make_piece(WHITE, piece_type)) - 
                    position.piece_count(make_piece(BLACK, piece_type)));

        /* Check if compiler unrolls first layer of loop */
        for(PieceType piece_type = PAWN; piece_type != KING; ++piece_type) 
        {
            eval += accumulate_pst(position, piece_type, WHITE, PS_TABLES[piece_type]) -
                    accumulate_pst(position, piece_type, BLACK, PS_TABLES[piece_type]);
        }
        
        const auto& king_table = is_endgame(position) ? KING_EG_PST : KING_MG_PST;
        
        eval += king_table[LSB(position.piece_bitboard(WHITE, KING))] - 
                king_table[LSB(position.piece_bitboard(BLACK, KING))];

        /* Pick a better name? */
        int32_t side_to_move = position.side_to_move() == WHITE ? 1 : -1;
        
        return eval * side_to_move;
    }
}