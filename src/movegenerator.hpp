#ifndef ALTAIR_MOVEGENERATOR_HPP
#define ALTAIR_MOVEGENERATOR_HPP

#include <vector>

#include "move.hpp"
#include "position.hpp"

class MoveGenerator
{
    public:
    /* Maybe parametrize the class itself */
    enum class Config
    {
        GENERATE_ALL,
        GENERATE_TACTICAL
    };

    std::vector<Move> all_moves(const Position& position);
    std::vector<Move> tactical_moves(const Position& position);

    private:
    std::vector<Move> moves_;

    private:
    template<Config config>
    void gen_moves_(const Position& position);

    /* Convenience functions for generating moves for individual pieces and special cases */

    template<Config config, Color side_to_play>
    void gen_pawn_moves_(const Position& position);

    template<Config config, PieceType piece_type>
    void gen_piece_moves_(const Position& position);

    void gen_castling_moves_(const Position& position);

    template<Config config, PieceType piece_type>
    void add_piece_moves_(const Position& position, Square from, BitBoard attacks);
};

#endif // ALTAIR_MOVEGENERATOR_HPP