#include "move.hpp"

Move::Move(uint32_t move)
: move_{move} { }


Square Move::from() const
{
    return (move_ >> 6) & 0x3F;
}

Square Move::to() const
{
    return move_ & 0x3F;
}

PieceType Move::piece_type() const
{
    return (move_ >> 12) & 0x7;
}


void Move::set_from(Square from)
{
    uint32_t mask = 0x3F << 6;
    move_ &= ~mask;
    move_ |= (from << 6);
}

void Move::set_to(Square to)
{
    uint32_t mask = 0x3F;
    move_ &= ~mask;
    move_ |= to;
}

void Move::set_piece_type(PieceType type)
{
    uint32_t mask = 0x7 << 12;
    move_ &= ~mask;
    move_ |= (type << 12);
}


std::string Move::notation() const
{
    std::string notation;
    
    return notation;
}