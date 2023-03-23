#include "position.hpp"

Position::Position()
{

}

Position::Position(const std::string& fen)
{

}


void Position::set_to_starting()
{
    
}

void Position::set_to_fen(std::string& fen)
{

}

std::string Position::fen() const
{

}


void Position::do_move(Move move)
{

}


BitBoard Position::piece_bitboard(Color color, PieceType type) const
{

}

BitBoard Position::all_pieces_bitboard() const
{

}


Piece Position::piece_occupying(Square square) const
{

}


Color Position::side_to_play() const
{
    return side_to_play_;
}

CastlingRights Position::castling_rights() const
{
    return castling_rights_;
}

Square Position::en_passant_square() const
{

}

uint32_t Position::halfclock() const
{
    return halfclock_;
}