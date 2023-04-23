#include "movegenerator.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "patterns.hpp"

#include <iostream>


std::vector<Move> MoveGenerator::all_moves(const Position& position)
{
    gen_moves_<Config::GENERATE_ALL>(position);
    return all_moves_;
}

std::vector<Move> MoveGenerator::tactical_moves(const Position& position)
{
    gen_moves_<Config::GENERATE_TACTICAL>(position);
    return tactical_moves_;
}


template<MoveGenerator::Config config>
void MoveGenerator::gen_moves_(const Position& position)
{
    gen_piece_moves_<config, KNIGHT >(position);
    gen_piece_moves_<config, BISHOP >(position);
    gen_piece_moves_<config, ROOK   >(position);
    gen_piece_moves_<config, QUEEN  >(position);
    gen_piece_moves_<config, KING   >(position);
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

        if constexpr (config == Config::GENERATE_ALL)
            all_moves_.push_back(move);
        else if(config == Config::GENERATE_TACTICAL)
            tactical_moves_.push_back(move);
    }
}