#include "search.hpp"
#include "movegenerator.hpp"
#include "eval.hpp"

#include <limits>
#include <iostream>
#include <format>

namespace Search
{
    constexpr uint8_t SEARCH_DEPTH = 6;
    constexpr int32_t STALEMATE_SCORE = 0;
    constexpr int32_t MATE_SCORE = -15000;

    Position::IrrecoverableState irrecoverable_states_[MAX_DEPTH];

    int32_t negamax_(Position &position, int current_depth)
    {
        if (current_depth == 0)
            return Evaluation::evaluate(position);

        std::vector<Move> moves;
        moves.reserve(MAX_MOVE_ARRAY_SIZE);

        MoveGenerator move_generator_{&position, &moves};
        move_generator_.gen_all_moves();

        int32_t max_score = MATE_SCORE;

        uint32_t legal_move_counter = 0;
        for (auto move : moves)
        {
            irrecoverable_states_[current_depth] = position.irrecoverable_state();
            position.make_move(move);

            if (!position.is_in_check(toggle_color(position.side_to_move())))
            {
                ++legal_move_counter;
                int32_t score = -negamax_(position, current_depth - 1);
                if (score > max_score)
                    max_score = score;
            }
            position.unmake_move(move, irrecoverable_states_[current_depth]);
        }

        if (legal_move_counter == 0)
        {
            if (position.is_in_check(position.side_to_move()))
                return MATE_SCORE - SEARCH_DEPTH + current_depth;
            return STALEMATE_SCORE;
        }

        return max_score;
    }

    Move search(Position &position)
    {
        std::vector<Move> moves;
        moves.reserve(MAX_MOVE_ARRAY_SIZE);

        MoveGenerator move_generator_{&position, &moves};
        move_generator_.gen_all_moves();

        int32_t max_score = MATE_SCORE;
        Move best_move;

        for (auto move : moves)
        {
            irrecoverable_states_[SEARCH_DEPTH] = position.irrecoverable_state();
            position.make_move(move);

            if (!position.is_in_check(toggle_color(position.side_to_move())))
            {
                int32_t score = -negamax_(position, SEARCH_DEPTH - 1);
                if (score > max_score)
                {
                    max_score = score;
                    best_move = move;
                }
            }

            position.unmake_move(move, irrecoverable_states_[SEARCH_DEPTH]);
        }
        return best_move;
    }
}