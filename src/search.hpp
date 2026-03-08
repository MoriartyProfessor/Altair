#ifndef GWAIHIR_SEARCH_HPP
#define GWAIHIR_SEARCH_HPP

#include "move.hpp"
#include "position.hpp"

namespace Search
{
    Move iterative_deepening_search(Position &position);
}

#endif // GWAIHIR_SEARCH_HPP