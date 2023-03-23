#ifndef ALTAIR_MOVEGENERATOR_HPP
#define ALTAIR_MOVEGENERATOR_HPP

#include <vector>

#include "move.hpp"
#include "position.hpp"

class MoveGenerator
{
    explicit MoveGenerator(const Position& position);

    void set_position(const Position& position);

    std::vector<Move> legal_moves();
    std::vector<Move> pseudolegal_moves();

    private:
    Position position_;
    std::vector<Move> legal_moves_;
    std::vector<Move> pseudolegal_moves_;

    /* Convenience functions for generating moves for individual pieces and special cases */
};

#endif // ALTAIR_MOVEGENERATOR_HPP