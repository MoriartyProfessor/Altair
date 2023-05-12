#include "movegenerator.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "patterns.hpp"

#include <iostream>

MoveGenerator::MoveGenerator(const Position* position, std::vector<Move>* moves)
: position_{position}, moves_{moves}
{

}

void MoveGenerator::gen_all_moves()
{
    gen_moves_<Config::GENERATE_ALL>();
}

void MoveGenerator::gen_tactical_moves()
{
    gen_moves_<Config::GENERATE_TACTICAL>();
}


template<MoveGenerator::Config config>
void MoveGenerator::gen_moves_()
{
    if(position_->side_to_move() == WHITE)
        gen_pawn_moves_ <config, WHITE>();
    else
        gen_pawn_moves_ <config, BLACK>();
    gen_piece_moves_<config, KNIGHT >();
    gen_piece_moves_<config, BISHOP >();
    gen_piece_moves_<config, ROOK   >();
    gen_piece_moves_<config, QUEEN  >();
    gen_piece_moves_<config, KING   >();
    
    if constexpr (config == Config::GENERATE_ALL)
        gen_castling_moves_();
}

template<MoveGenerator::Config config, Color side_to_play>
void MoveGenerator::gen_pawn_moves_()
{
    /* Refactor */
    static_assert(side_to_play < N_COLORS);
    BitBoard pawn_bitboard = position_->piece_bitboard(side_to_play, PAWN);
    
    BitBoard push_targets, double_push_targets, left_attacks, right_attacks, promotion_push_targets, promotion_left_attacks, promotion_right_attacks;

    constexpr BitBoard starting_rank_BB  = side_to_play == WHITE ? RANK_2_BB : RANK_7_BB;
    constexpr BitBoard promotion_rank_BB = side_to_play == WHITE ? RANK_8_BB : RANK_1_BB;
    constexpr Direction r_NORTH       = side_to_play == WHITE ? NORTH         : SOUTH;
    constexpr Direction r_NORTH_WEST  = side_to_play == WHITE ? NORTH_WEST    : SOUTH_EAST;
    constexpr Direction r_WEST        = side_to_play == WHITE ? WEST          : EAST;
    constexpr Direction r_SOUTH_WEST  = side_to_play == WHITE ? SOUTH_WEST    : NORTH_EAST;
    constexpr Direction r_SOUTH       = side_to_play == WHITE ? SOUTH         : NORTH;
    constexpr Direction r_SOUTH_EAST  = side_to_play == WHITE ? SOUTH_EAST    : NORTH_WEST;
    constexpr Direction r_EAST        = side_to_play == WHITE ? EAST          : WEST;
    constexpr Direction r_NORTH_EAST  = side_to_play == WHITE ? NORTH_EAST    : SOUTH_WEST;

    
    push_targets = BitBoards::step<r_NORTH>(pawn_bitboard) & ~position_->occupancy_bitboard();
    double_push_targets = BitBoards::step<r_NORTH>(starting_rank_BB & pawn_bitboard) & ~position_->occupancy_bitboard();
    double_push_targets = BitBoards::step<r_NORTH>(double_push_targets) & ~position_->occupancy_bitboard();
    left_attacks = BitBoards::step<r_NORTH_WEST>(pawn_bitboard) & position_->occupancy_bitboard(toggle_color(side_to_play));
    right_attacks = BitBoards::step<r_NORTH_EAST>(pawn_bitboard) & position_->occupancy_bitboard(toggle_color(side_to_play));

    promotion_push_targets = push_targets & promotion_rank_BB;
    promotion_left_attacks = left_attacks & promotion_rank_BB;
    promotion_right_attacks = right_attacks & promotion_rank_BB;
    push_targets &= ~promotion_rank_BB; left_attacks &= ~promotion_rank_BB; right_attacks &= ~promotion_rank_BB;

    if constexpr (config == Config::GENERATE_ALL)
    {
        for(Square push_sq = pop_LSB(push_targets); push_sq != N_SQUARES; push_sq = pop_LSB(push_targets))
        {
            Move move{step<r_SOUTH>(push_sq), push_sq};

            move.set_quite();
            move.set_piece_type(PAWN);
            moves_->push_back(move);
        }

        for(Square double_push_sq = pop_LSB(double_push_targets); double_push_sq != N_SQUARES; double_push_sq = pop_LSB(double_push_targets))
        {
            Move move{step<r_SOUTH>(double_push_sq, 2), double_push_sq};

            move.set_double_pawn_push();
            move.set_piece_type(PAWN);
            moves_->push_back(move);
        }
    }
    
    for(Square attack = pop_LSB(left_attacks); attack != N_SQUARES; attack = pop_LSB(left_attacks))
    {
        Move move{step<r_SOUTH_EAST>(attack), attack};

        move.set_capture();
        move.set_piece_type(PAWN);
        
        Piece attacked_piece = position_->piece_occupying(attack);
        move.set_capture_piece_type(get_type(attacked_piece));
        moves_->push_back(move);
    }

    for(Square attack = pop_LSB(right_attacks); attack != N_SQUARES; attack = pop_LSB(right_attacks))
    {
        Move move{step<r_SOUTH_WEST>(attack), attack};

        move.set_capture();
        move.set_piece_type(PAWN);

        Piece attacked_piece = position_->piece_occupying(attack);
        move.set_capture_piece_type(get_type(attacked_piece));
        moves_->push_back(move);
    }

    for(Square promotion_push_sq = pop_LSB(promotion_push_targets); promotion_push_sq != N_SQUARES; promotion_push_sq = pop_LSB(promotion_push_targets))
    {
        Move move{step<r_SOUTH>(promotion_push_sq), promotion_push_sq};

        move.set_promotion();
        move.set_piece_type(PAWN);
        for(PieceType promotion_type = KNIGHT; promotion_type <= QUEEN; ++promotion_type)
        {
            move.set_promotion_piece_type(promotion_type);
            moves_->push_back(move);
        }
    }

    for(Square promotion_attack = pop_LSB(promotion_left_attacks); promotion_attack != N_SQUARES; promotion_attack = pop_LSB(promotion_left_attacks))
    {
        Move move{step<r_SOUTH_EAST>(promotion_attack), promotion_attack};

        move.set_promotion();
        move.set_piece_type(PAWN);

        move.set_capture();
        Piece attacked_piece = position_->piece_occupying(promotion_attack);
        move.set_capture_piece_type(get_type(attacked_piece));

        for(PieceType promotion_type = KNIGHT; promotion_type <= QUEEN; ++promotion_type)
        {
            move.set_promotion_piece_type(promotion_type);
            moves_->push_back(move);
        }
    }

    for(Square promotion_attack = pop_LSB(promotion_right_attacks); promotion_attack != N_SQUARES; promotion_attack = pop_LSB(promotion_right_attacks))
    {
        Move move{step<r_SOUTH_WEST>(promotion_attack), promotion_attack};

        move.set_promotion();
        move.set_piece_type(PAWN);

        move.set_capture();
        Piece attacked_piece = position_->piece_occupying(promotion_attack);
        move.set_capture_piece_type(get_type(attacked_piece));

        for(PieceType promotion_type = KNIGHT; promotion_type <= QUEEN; ++promotion_type)
        {
            move.set_promotion_piece_type(promotion_type);
            moves_->push_back(move);
        }
    }

    if(position_->en_passant_square() != N_SQUARES)
    {
        BitBoard en_passant_sources = Patterns::get_pawn_attacks(position_->en_passant_square(), toggle_color(side_to_play)) & position_->piece_bitboard(side_to_play, PAWN);
        if(en_passant_sources)
        {
            Move move{LSB(en_passant_sources), position_->en_passant_square()};
            move.set_en_passant();
            moves_->push_back(move);
        }
    }
}


template<MoveGenerator::Config config, PieceType piece_type>
void MoveGenerator::gen_piece_moves_()
{
    static_assert(piece_type < N_PIECE_TYPES);
    BitBoard piece_bitboard = position_->piece_bitboard(position_->side_to_move(), piece_type);

    for(Square piece_sq = pop_LSB(piece_bitboard); piece_sq != N_SQUARES; piece_sq = pop_LSB(piece_bitboard))
    {
        BitBoard piece_attacks = EMPTY_BB;
        if constexpr (piece_type == KNIGHT)
            piece_attacks = Patterns::get_knight_attacks(piece_sq) & ~position_->occupancy_bitboard(position_->side_to_move());
        else if (piece_type == BISHOP)
            piece_attacks = Patterns::get_bishop_attacks(piece_sq, position_->occupancy_bitboard()) & ~position_->occupancy_bitboard(position_->side_to_move());
        else if (piece_type == ROOK)
            piece_attacks = Patterns::get_rook_attacks(piece_sq, position_->occupancy_bitboard()) & ~position_->occupancy_bitboard(position_->side_to_move());
        else if (piece_type == QUEEN)
            piece_attacks = Patterns::get_queen_attacks(piece_sq, position_->occupancy_bitboard()) & ~position_->occupancy_bitboard(position_->side_to_move());
        else if (piece_type == KING)
            piece_attacks = Patterns::get_king_attacks(piece_sq) & ~position_->occupancy_bitboard(position_->side_to_move());
        
        add_piece_moves_<config, piece_type>(piece_sq, piece_attacks);
    }
}

void MoveGenerator::gen_castling_moves_()
{
    Color side_to_move = position_->side_to_move();
    
    if(position_->is_in_check(side_to_move))
        return;
    
    CastlingRights rights = position_->castling_rights();
    
    Square  king_square = side_to_move == WHITE ? SQ_E1 : SQ_E8,
            queen_rook_square = side_to_move == WHITE ? SQ_A1 : SQ_A8,
            king_rook_square = side_to_move == WHITE ? SQ_H1 : SQ_H8;

    if(rights.queen_side(side_to_move))
    {
        BitBoard queen_side_mask = QUEEN_SIDE_CASTLE_MASKS_BB[side_to_move];
        bool permission =   !(((queen_side_mask & position_->occupancy_bitboard()) != EMPTY_BB) ||
                            position_->is_square_attacked(pop_LSB(queen_side_mask), toggle_color(side_to_move)) ||
                            position_->is_square_attacked(pop_LSB(queen_side_mask), toggle_color(side_to_move)) ||
                            position_->is_square_attacked(pop_LSB(queen_side_mask), toggle_color(side_to_move)));

        if(permission)
        {
            Move move{king_square, queen_rook_square};
            move.set_queen_side_castle();
            moves_->push_back(move);
        }
    }

    if(rights.king_side(side_to_move))
    {
        BitBoard king_side_mask = KING_SIDE_CASTLE_MASKS_BB[position_->side_to_move()];
        bool permission =   !(((king_side_mask & position_->occupancy_bitboard()) != EMPTY_BB) ||
                            position_->is_square_attacked(pop_LSB(king_side_mask), toggle_color(side_to_move)) ||
                            position_->is_square_attacked(pop_LSB(king_side_mask), toggle_color(side_to_move)));
        if(permission)
        {
            Move move{king_square, king_rook_square};
            move.set_king_side_castle();
            moves_->push_back(move);
        }
    }
}

template<MoveGenerator::Config config, PieceType piece_type>
void MoveGenerator::add_piece_moves_(Square from, BitBoard attacks)
{
    for(Square attack = pop_LSB(attacks); attack != N_SQUARES; attack = pop_LSB(attacks))
    {
        Move move{from, attack};
        move.set_piece_type(piece_type);

        Piece attacked_piece = position_->piece_occupying(attack);
        if(attacked_piece != N_PIECES)
        {
            move.set_capture();
            move.set_capture_piece_type(get_type(attacked_piece));
        }
        else
        {
            if constexpr(config == Config::GENERATE_TACTICAL)
                continue; // This is very bad
            
            move.set_quite();
        }

        moves_->push_back(move);
    }
}