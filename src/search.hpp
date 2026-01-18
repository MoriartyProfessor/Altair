#ifndef ALTAIR_SEARCH_HPP
#define ALTAIR_SEARCH_HPP

#include "move.hpp"
#include "position.hpp"

namespace Search
{
    struct SearchInfo
    {
        int depth;
        int score;
        Move best_move;
        bool is_mate;
        bool is_draw;
    };
    Move search(Position &position);
}

#endif // ALTAIR_SEARCH_HPP