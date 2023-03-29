#include <cmath>
#include <sstream>

#include "moveparser.hpp"

CoordinateMoveParser::CoordinateMoveParser(const Position& position)
: position_{position}
{

}
    
void CoordinateMoveParser::set_position(const Position& position)
{
    position_ = position;
}

Move CoordinateMoveParser::parse_move(const std::string& descriptor)
{
    Square from = str_to_square(descriptor.substr(0, 2));
    Square to = str_to_square(descriptor.substr(2, 2));
    
    Move move{from, to};

    Piece moving_piece = position_.piece_occupying(from);
    move.set_piece_type(get_type(moving_piece));

    if(descriptor.size() > 4)
    {
        move.set_promotion();
        move.set_promotion_piece_type(get_type(char_to_piece(descriptor[4])));
    }
    else if(position_.piece_occupying(to) != N_PIECES)
    {
        move.set_capture();
        move.set_capture_piece_type(get_type(position_.piece_occupying(to)));
    }
    else if(move.piece_type() == KING && (from == SQ_E1 && to == SQ_G1) || (from == SQ_E8 && to == SQ_G8))
    {
        move.set_king_side_castle();
    }
    else if(move.piece_type() == KING && (from == SQ_E1 && to == SQ_C1) || (from == SQ_E8 && to == SQ_C8))
    {
        move.set_queen_side_castle();
    }
    else if(move.piece_type() == PAWN && to == position_.en_passant_square())
    {
        move.set_en_passant();
    }
    else if(move.piece_type() == PAWN && abs(from - to) == 16)
    {
        move.set_double_pawn_push();
    }
    else if(!move.is_promotion())
    {
        move.set_quite();
    }
    return move;
}

std::vector<Move> CoordinateMoveParser::parse_sequence(const std::string& descriptor)
{
    Position tmp_position = position_;
    std::vector<Move> move_sequence;
    std::stringstream move_stream(descriptor);
    std::string move_descriptor;

    while(!move_stream.eof())
    {
        move_stream >> move_descriptor;
        Move next_move = parse_move(move_descriptor);
        position_.make_move(next_move);
        move_sequence.push_back(next_move);
    }
    position_ = tmp_position;
    return move_sequence;
}