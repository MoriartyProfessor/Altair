#ifndef ALTAIR_MOVE_HPP
#define ALTAIR_MOVE_HPP

#include "defs.hpp"

class Move
{
    public:

    explicit Move(uint32_t move);

    PieceType piece_type();

    PieceType color();

    PieceType from();

    private:
    uint32_t move_;
};


#endif // ALTAIR_MOVE_HPP