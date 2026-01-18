#ifndef ALTAIR_MOVEORDERER_HPP
#define ALTAIR_MOVEORDERER_HPP

#include "movegenerator.hpp"

#include <vector>

class MoveOrderer
{
    public:
    MoveOrderer(MoveList* moves);
    Move next();
    bool has_next() const;

    private:
    void assign_scores_();

    MoveList& moves_;
    std::vector<uint32_t> scores_;
    std::uint16_t current_index_ = 0;
};

#endif // ALTAIR_MOVEORDERER_HPP