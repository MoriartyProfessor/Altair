#include "move.hpp"

Move::Move(uint32_t move)
: move_{move & 0x0FFFFFFF} { }

Move::Move(Square from, Square to)
{
    set_from(from);
    set_to(to);
}

Move& Move::operator=(Move other) 
{
    move_ = other.move_;
    return *this;
}

bool Move::operator==(Move other) const 
{
    return move_ == other.move_;
}
   
bool Move::operator!=(Move other) const 
{
    return move_ != other.move_;
}


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

PieceType Move::capture_piece_type() const
{
    return (move_ >> 15) & 0x7;
}

PieceType Move::promotion_piece_type() const
{
    return (move_ >> 18) & 0x7;
}

bool Move::is_quite()               const { return (move_ >> 21) & (1 << QUITE); }
bool Move::is_capture()             const { return (move_ >> 21) & (1 << CAPTURE); }
bool Move::is_promotion()           const { return (move_ >> 21) & (1 << PROMOTION); }
bool Move::is_double_pawn_push()    const { return (move_ >> 21) & (1 << DOUBLE_PAWN_PUSH); }
bool Move::is_king_side_castle()    const { return (move_ >> 21) & (1 << KING_CASTLE); }
bool Move::is_queen_side_castle()   const { return (move_ >> 21) & (1 << QUEEN_CASTLE); }
bool Move::is_en_passant()          const { return (move_ >> 21) & (1 << EN_PASSANT); }


void Move::set_from(Square from)
{
    uint32_t mask = 0x3F << 6;
    move_ &= ~mask;
    move_ |= mask & (from << 6);
}

void Move::set_to(Square to)
{
    uint32_t mask = 0x3F;
    move_ &= ~mask;
    move_ |= mask & to;
}

void Move::set_piece_type(PieceType type)
{
    uint32_t mask = 0x7 << 12;
    move_ &= ~mask;
    move_ |= mask & (type << 12);
}

void Move::set_capture_piece_type(PieceType type)
{
    uint32_t mask = 0x7 << 15;
    move_ &= ~mask;
    move_ |= mask & (type << 15);
}

void Move::set_promotion_piece_type(PieceType type)
{
    uint32_t mask = 0x7 << 18;
    move_ &= ~mask;
    move_ |= mask & (type << 18);
}

void Move::set_quite()              { set_flag(QUITE); }
void Move::set_capture()            { set_flag(CAPTURE); }
void Move::set_promotion()          { set_flag(PROMOTION); }
void Move::set_double_pawn_push()   { set_flag(DOUBLE_PAWN_PUSH); }
void Move::set_king_side_castle()   { set_flag(KING_CASTLE); }
void Move::set_queen_side_castle()  { set_flag(QUEEN_CASTLE); }
void Move::set_en_passant()         { set_flag(EN_PASSANT); }

void Move::set_flag(FlagBit flag)
{
    uint32_t mask = 0x7F << 21;
    move_ &= ~mask;
    move_ |= mask & (1 << (21 + flag));
}


std::string Move::descriptive_notation() const
{
    std::string notation;
    
    return notation;
}

std::string Move::algebraic_notation() const
{
    return square_to_str(from()) + square_to_str(to());
}