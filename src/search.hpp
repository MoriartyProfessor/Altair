#ifndef GWAIHIR_SEARCH_HPP
#define GWAIHIR_SEARCH_HPP

#include "move.hpp"
#include "position.hpp"

namespace Search
{
    // Should these be in Search?
    void update_history_stack(Zobrist::HashKey key, uint32_t ply);
    void clear_history_stack();
    Move iterative_deepening_search(Position &position);
}

#endif // GWAIHIR_SEARCH_HPP