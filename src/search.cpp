#include "search.hpp"
#include "movegenerator.hpp"
#include "moveorderer.hpp"
#include "eval.hpp"
#include "transpositiontable.hpp"

#include <limits>
#include <iostream>
#include <format>

namespace Search
{
    constexpr uint8_t SEARCH_DEPTH = 9;
    constexpr int32_t STALEMATE_SCORE = 0;
    constexpr int32_t MATE_SCORE = -15000;

    Position::IrrecoverableState irrecoverable_states_[MAX_DEPTH];

    TranspostionTable transposition_table{4_M};

    struct Result
    {
        int score;
        Move move;
        Result operator-() {return { -score, move };}
    };

    Move get_hash_move(const Position& position)
    {
        Move hash_move;
        
        const auto & entry = transposition_table.probe(position.hashkey());
        if(!entry)
            return hash_move;
        if(entry->node_type != TTEntry::NodeType::EXACT)
            return hash_move;
        return entry->best_move;
    }

    Result negamax(Position position, int alpha, int beta, int depth)
    {
        if (depth == 0)
            return {Evaluation::evaluate(position), {}};

        MoveList moves;

        MoveGenerator move_generator_{&position, &moves};
        move_generator_.gen_all_moves();

        MoveOrderer move_orderer{moves, get_hash_move(position)};

        Result best_result {MATE_SCORE, {}};

        uint32_t legal_move_counter = 0;
        while (move_orderer.has_next())
        {
            Move move = move_orderer.next();
            
            position.make_move(move);

            Position::IrrecoverableState irrecoverable_state = position.irrecoverable_state();

            if (!position.is_in_check(toggle_color(position.side_to_move())))
            {
                ++legal_move_counter;
                auto result = -negamax(position, -beta, -alpha, depth - 1);
                result.move = move;
                if (result.score > best_result.score)
                {
                    best_result = result;
                    if( result.score > alpha ) 
                        alpha = result.score;
                }
                if(result.score >= beta)
                    return best_result;
            }

            position.unmake_move(move, irrecoverable_state);
        }

        if (legal_move_counter == 0)
        {
            if (position.is_in_check(position.side_to_move()))
                best_result.score =  MATE_SCORE - SEARCH_DEPTH + depth;
            else
                best_result.score = STALEMATE_SCORE;
        }

        TTEntry tt_entry = {.key = position.hashkey(), 
                            .best_move = best_result.move, 
                            .depth = depth,
                            .score = best_result.score,
                            .node_type = TTEntry::NodeType::EXACT};
                    transposition_table.insert(std::move(tt_entry));

        return best_result;
    }

    Move iterative_deepening_search(Position &position)
    {
        Result result;

        for(unsigned depth = 1; depth <= SEARCH_DEPTH; ++depth)
        {
            result = negamax(position, MATE_SCORE, -MATE_SCORE, depth);
            std::cout << std::format("[Debug] Depth: {}, Move: {}, Score: {}", depth, result.move.uci_notation(), result.score) << std::endl;
        }
        return result.move;
    }
}