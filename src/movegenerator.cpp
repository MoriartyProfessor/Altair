#include "movegenerator.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "patterns.hpp"

#include <iostream>


std::vector<Move> MoveGenerator::all_moves(const Position& position)
{
    gen_moves_<Config::GENERATE_ALL>(position);
    /* Where to create move list? */
    auto moves = std::move(moves_);
    moves_.reserve(MOVE_LIST_SIZE);
    return moves;
}

std::vector<Move> MoveGenerator::tactical_moves(const Position& position)
{
    gen_moves_<Config::GENERATE_TACTICAL>(position);
    auto moves = std::move(moves_);
    moves_.reserve(MOVE_LIST_SIZE);
    return moves;
}


template<MoveGenerator::Config config>
void MoveGenerator::gen_moves_(const Position& position)
{
    if(position.side_to_move() == WHITE)
        gen_pawn_moves_ <config, WHITE>(position);
    else
        gen_pawn_moves_ <config, BLACK>(position);
    gen_piece_moves_<config, KNIGHT >(position);
    gen_piece_moves_<config, BISHOP >(position);
    gen_piece_moves_<config, ROOK   >(position);
    gen_piece_moves_<config, QUEEN  >(position);
    gen_piece_moves_<config, KING   >(position);
    
    if constexpr (config == Config::GENERATE_ALL)
        gen_castling_moves_(position);
}

template<MoveGenerator::Config config, Color side_to_play>
void MoveGenerator::gen_pawn_moves_(const Position& position)
{
    /* Refactor */
    static_assert(side_to_play < N_COLORS);
    BitBoard pawn_bitboard = position.piece_bitboard(side_to_play, PAWN);
    
    BitBoard starting_rank_BB, promotion_rank_BB;
    BitBoard push_targets, double_push_targets, left_attacks, right_attacks, promotion_push_targets, promotion_left_attacks, promotion_right_attacks;

    if constexpr (side_to_play == WHITE)
    {
        starting_rank_BB = RANK_2_BB;
        promotion_rank_BB = RANK_8_BB;
        push_targets = BitBoards::step<NORTH>(pawn_bitboard) & ~position.occupancy_bitboard();
        double_push_targets = BitBoards::step<NORTH>(starting_rank_BB & pawn_bitboard) & ~position.occupancy_bitboard();
        double_push_targets = BitBoards::step<NORTH>(double_push_targets) & ~position.occupancy_bitboard();
        left_attacks = BitBoards::step<NORTH_WEST>(pawn_bitboard) & position.occupancy_bitboard(toggle_color(side_to_play));
        right_attacks = BitBoards::step<NORTH_EAST>(pawn_bitboard) & position.occupancy_bitboard(toggle_color(side_to_play));
    }
    else
    {
        starting_rank_BB = RANK_7_BB;
        promotion_rank_BB = RANK_1_BB;
        push_targets = BitBoards::step<SOUTH>(pawn_bitboard) & ~position.occupancy_bitboard();
        double_push_targets = BitBoards::step<SOUTH>(starting_rank_BB & pawn_bitboard) & ~position.occupancy_bitboard();
        double_push_targets = BitBoards::step<SOUTH>(double_push_targets) & ~position.occupancy_bitboard();
        left_attacks = BitBoards::step<SOUTH_EAST>(pawn_bitboard) & position.occupancy_bitboard(toggle_color(side_to_play));
        right_attacks = BitBoards::step<SOUTH_WEST>(pawn_bitboard) & position.occupancy_bitboard(toggle_color(side_to_play));
    }

    promotion_push_targets = push_targets & promotion_rank_BB;
    promotion_left_attacks = left_attacks & promotion_rank_BB;
    promotion_right_attacks = right_attacks & promotion_rank_BB;
    push_targets &= ~promotion_rank_BB; left_attacks &= ~promotion_rank_BB; right_attacks &= ~promotion_rank_BB;

    for(Square push_sq = pop_LSB(push_targets); push_sq != N_SQUARES; push_sq = pop_LSB(push_targets))
    {
        Move move{step<SOUTH>(push_sq), push_sq};
        
        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH>(push_sq));

        move.set_quite();
        move.set_piece_type(PAWN);
        moves_.push_back(move);
    }

    for(Square double_push_sq = pop_LSB(double_push_targets); double_push_sq != N_SQUARES; double_push_sq = pop_LSB(double_push_targets))
    {
        Move move{step<SOUTH>(double_push_sq, 2), double_push_sq};

        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH>(double_push_sq, 2));

        move.set_double_pawn_push();
        move.set_piece_type(PAWN);
        moves_.push_back(move);
    }
    
    for(Square attack = pop_LSB(left_attacks); attack != N_SQUARES; attack = pop_LSB(left_attacks))
    {
        Move move{step<SOUTH_EAST>(attack), attack};

        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH_WEST>(attack));

        move.set_capture();
        move.set_piece_type(PAWN);
        
        Piece attacked_piece = position.piece_occupying(attack);
        move.set_capture_piece_type(get_type(attacked_piece));
        moves_.push_back(move);
    }

    for(Square attack = pop_LSB(right_attacks); attack != N_SQUARES; attack = pop_LSB(right_attacks))
    {
        Move move{step<SOUTH_WEST>(attack), attack};

        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH_EAST>(attack));

        move.set_capture();
        move.set_piece_type(PAWN);

        Piece attacked_piece = position.piece_occupying(attack);
        move.set_capture_piece_type(get_type(attacked_piece));
        moves_.push_back(move);
    }

    for(Square promotion_push_sq = pop_LSB(promotion_push_targets); promotion_push_sq != N_SQUARES; promotion_push_sq = pop_LSB(promotion_push_targets))
    {
        Move move{step<SOUTH>(promotion_push_sq), promotion_push_sq};

        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH>(promotion_push_sq));

        move.set_promotion();
        move.set_piece_type(PAWN);
        for(PieceType promotion_type = KNIGHT; promotion_type < N_PIECE_TYPES; ++promotion_type)
        {
            move.set_promotion_piece_type(promotion_type);
            moves_.push_back(move);
        }
    }

    for(Square promotion_attack = pop_LSB(promotion_left_attacks); promotion_attack != N_SQUARES; promotion_attack = pop_LSB(promotion_left_attacks))
    {
        Move move{step<SOUTH_EAST>(promotion_attack), promotion_attack};

        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH_WEST>(promotion_attack));

        move.set_promotion();
        move.set_piece_type(PAWN);

        move.set_capture();
        Piece attacked_piece = position.piece_occupying(promotion_attack);
        move.set_capture_piece_type(get_type(attacked_piece));

        for(PieceType promotion_type = KNIGHT; promotion_type < N_PIECE_TYPES; ++promotion_type)
        {
            move.set_promotion_piece_type(promotion_type);
            moves_.push_back(move);
        }
    }

    for(Square promotion_attack = pop_LSB(promotion_right_attacks); promotion_attack != N_SQUARES; promotion_attack = pop_LSB(promotion_right_attacks))
    {
        Move move{step<SOUTH_WEST>(promotion_attack), promotion_attack};

        if constexpr (side_to_play == BLACK)
            move.set_from(step<NORTH_EAST>(promotion_attack));

        move.set_promotion();
        move.set_piece_type(PAWN);

        move.set_capture();
        Piece attacked_piece = position.piece_occupying(promotion_attack);
        move.set_capture_piece_type(get_type(attacked_piece));

        for(PieceType promotion_type = KNIGHT; promotion_type < N_PIECE_TYPES; ++promotion_type)
        {
            move.set_promotion_piece_type(promotion_type);
            moves_.push_back(move);
        }
    }

    if(position.en_passant_square() != N_SQUARES)
    {
        BitBoard en_passant_sources = Patterns::get_pawn_attacks(position.en_passant_square(), toggle_color(side_to_play)) & position.piece_bitboard(side_to_play, PAWN);
        if(en_passant_sources)
        {
            Move move{LSB(en_passant_sources), position.en_passant_square()};
            move.set_en_passant();
            moves_.push_back(move);
        }
    }
}


template<MoveGenerator::Config config, PieceType piece_type>
void MoveGenerator::gen_piece_moves_(const Position& position)
{
    static_assert(piece_type < N_PIECE_TYPES);
    BitBoard piece_bitboard = position.piece_bitboard(position.side_to_move(), piece_type);

    for(Square piece_sq = pop_LSB(piece_bitboard); piece_sq != N_SQUARES; piece_sq = pop_LSB(piece_bitboard))
    {
        BitBoard piece_attacks = EMPTY_BB;
        if constexpr (piece_type == KNIGHT)
            piece_attacks = Patterns::get_knight_attacks(piece_sq) & ~position.occupancy_bitboard(position.side_to_move());
        else if (piece_type == BISHOP)
            piece_attacks = Patterns::get_bishop_attacks(piece_sq, position.occupancy_bitboard()) & ~position.occupancy_bitboard(position.side_to_move());
        else if (piece_type == ROOK)
            piece_attacks = Patterns::get_rook_attacks(piece_sq, position.occupancy_bitboard()) & ~position.occupancy_bitboard(position.side_to_move());
        else if (piece_type == QUEEN)
            piece_attacks = Patterns::get_queen_attacks(piece_sq, position.occupancy_bitboard()) & ~position.occupancy_bitboard(position.side_to_move());
        else if (piece_type == KING)
            piece_attacks = Patterns::get_king_attacks(piece_sq) & ~position.occupancy_bitboard(position.side_to_move());
        
        add_piece_moves_<config, piece_type>(position, piece_sq, piece_attacks);
    }
}

void MoveGenerator::gen_castling_moves_(const Position& position)
{
    Color side_to_move = position.side_to_move();
    
    if(position.is_in_check(side_to_move))
        return;
    
    CastlingRights rights = position.castling_rights();
    
    Square  king_square = side_to_move == WHITE ? SQ_E1 : SQ_E8,
            queen_rook_square = side_to_move == WHITE ? SQ_A1 : SQ_A8,
            king_rook_square = side_to_move == WHITE ? SQ_H1 : SQ_H8;

    if(rights.queen_side(side_to_move))
    {
        BitBoard queen_side_mask = QUEEN_SIDE_CASTLE_MASKS_BB[side_to_move];
        bool permission =   !(((queen_side_mask & position.occupancy_bitboard()) != EMPTY_BB) ||
                            position.is_square_attacked(pop_LSB(queen_side_mask), toggle_color(side_to_move)) ||
                            position.is_square_attacked(pop_LSB(queen_side_mask), toggle_color(side_to_move)) ||
                            position.is_square_attacked(pop_LSB(queen_side_mask), toggle_color(side_to_move)));

        if(permission)
        {
            Move move{king_square, queen_rook_square};
            move.set_queen_side_castle();
            moves_.push_back(move);
        }
    }

    if(rights.king_side(side_to_move))
    {
        BitBoard king_side_mask = KING_SIDE_CASTLE_MASKS_BB[position.side_to_move()];
        bool permission =   !(((king_side_mask & position.occupancy_bitboard()) != EMPTY_BB) ||
                            position.is_square_attacked(pop_LSB(king_side_mask), toggle_color(side_to_move)) ||
                            position.is_square_attacked(pop_LSB(king_side_mask), toggle_color(side_to_move)));
        if(permission)
        {
            Move move{king_square, king_rook_square};
            move.set_king_side_castle();
            moves_.push_back(move);
        }
    }
}

template<MoveGenerator::Config config, PieceType piece_type>
void MoveGenerator::add_piece_moves_(const Position& position, Square from, BitBoard attacks)
{
    for(Square attack = pop_LSB(attacks); attack != N_SQUARES; attack = pop_LSB(attacks))
    {
        Move move{from, attack};
        move.set_piece_type(piece_type);

        Piece attacked_piece = position.piece_occupying(attack);
        if(attacked_piece != N_PIECES)
        {
            move.set_capture();
            move.set_capture_piece_type(get_type(attacked_piece));
        }
        else
        {
            move.set_quite();
        }

        moves_.push_back(move);
    }
}