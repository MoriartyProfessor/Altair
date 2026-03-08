#ifndef GWAIHIR_MOVEORDERER_HPP
#define GWAIHIR_MOVEORDERER_HPP

#include "movegenerator.hpp"
#include "transpositiontable.hpp"

#include <boost/container/small_vector.hpp>

class MoveOrderer
{
    public:
    MoveOrderer(MoveList& moves, Move hash_move = Move{});
    Move next();
    bool has_next() const;

    private:
    void assign_scores_(Move hash_move);

    MoveList& moves_;
    boost::container::small_vector<uint32_t, MAX_MOVE_LIST_SIZE> scores_;
    std::uint16_t current_index_ = 0;
};

#endif // GWAIHIR_MOVEORDERER_HPP