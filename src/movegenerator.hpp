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

    MoveGenerator(const Position* position, std::vector<Move>* moves);

    void gen_all_moves();
    void gen_tactical_moves();

    private:
    template<Config config>
    void gen_moves_();

    /* Convenience functions for generating moves for individual pieces and special cases */

    template<Config config, Color side_to_play>
    void gen_pawn_moves_();

    template<Config config, PieceType piece_type>
    void gen_piece_moves_();

    void gen_castling_moves_();

    template<Config config, PieceType piece_type>
    void add_piece_moves_(Square from, BitBoard attacks);

    private:
    std::vector<Move>* moves_;
    const Position* position_;
};

#endif // ALTAIR_MOVEGENERATOR_HPP