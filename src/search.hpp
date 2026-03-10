#ifndef GWAIHIR_SEARCH_HPP
#define GWAIHIR_SEARCH_HPP

#include "move.hpp"
#include "position.hpp"

namespace Search
{
    // Encapsulate it into class
    void push_to_history_stack(Zobrist::HashKey key);
    void pop_from_history_stack();
    void clear_history_stack();
    Move iterative_deepening_search(Position &position);
}

#endif // GWAIHIR_SEARCH_HPP