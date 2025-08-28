#include "eval.hpp"
namespace Evaluation
{
    constexpr int32_t MATERIAL_VALUES[N_PIECE_TYPES] = {100, 350, 350, 525, 1000, 20000};

    int32_t evaluate(const Position &position)
    {
        int32_t eval = 0;
        eval += MATERIAL_VALUES[PAWN]   * (position.piece_count(WH_PAWN)   - position.piece_count(BL_PAWN));
        eval += MATERIAL_VALUES[KNIGHT] * (position.piece_count(WH_KNIGHT) - position.piece_count(BL_KNIGHT));
        eval += MATERIAL_VALUES[BISHOP] * (position.piece_count(WH_BISHOP) - position.piece_count(BL_BISHOP));
        eval += MATERIAL_VALUES[ROOK]   * (position.piece_count(WH_ROOK)   - position.piece_count(BL_ROOK));
        eval += MATERIAL_VALUES[QUEEN]  * (position.piece_count(WH_QUEEN)  - position.piece_count(BL_QUEEN));
        eval += MATERIAL_VALUES[KING]   * (position.piece_count(WH_KING)   - position.piece_count(BL_KING));

        /*Pick a better name?*/
        int32_t side_to_move = position.side_to_move() == WHITE ? 1 : -1;
        
        return eval * side_to_move;
    }
}