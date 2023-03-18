#ifndef ALTAIR_MOVE_HPP
#define ALTAIR_MOVE_HPP

#include <string>

#include "defs.hpp"

class Move
{
    public:

    Move() = default;
    Move(uint32_t move);

    Square from() const;
    Square to() const;
    PieceType piece_type() const;

    void set_from(Square from);
    void set_to(Square to);
    void set_piece_type(PieceType from);

    std::string notation() const;

    private:
    uint32_t move_ = 0;
};


#endif // ALTAIR_MOVE_HPP