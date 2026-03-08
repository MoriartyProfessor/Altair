#include "search.hpp"
#include "movegenerator.hpp"
#include "moveorderer.hpp"
#include "eval.hpp"
#include "transpositiontable.hpp"

#include <boost/container/small_vector.hpp>

#include <limits>
#include <iostream>
#include <format>

using HistoryStack = boost::container::small_vector<Zobrist::HashKey, 256>;

namespace Search
{
    constexpr uint8_t SEARCH_DEPTH = 9;
    constexpr int32_t STALEMATE_SCORE = 0;
    constexpr int32_t MATE_SCORE = -15000;

    Position::IrrecoverableState irrecoverable_states_[MAX_DEPTH];

    HistoryStack history_stack;

    TranspostionTable transposition_table{4_M};

    struct Result
    {
        int score;
        Move move;
        Result operator-() {return { -score, move };}
    };

    uint64_t node_count = 0;

    Move get_hash_move(const std::optional<TTEntry> &entry)
    {
        Move hash_move;
        
        if(!entry)
            return hash_move;
        return entry->best_move;
    }

    bool was_position_reached(const HistoryStack& history_stack, uint32_t ply)
    {
        auto current_position = history_stack[ply];
        for(int i = ply - 2; i >= 0; i -= 2)
        {
            if(history_stack[i] == current_position)
                return true;
        }
        return false;
    }

    void update_history_stack(Zobrist::HashKey key, uint32_t ply)
    {
        if(ply < history_stack.size())
            history_stack[ply] = key;
        else
            history_stack.push_back(key);
    }

    void clear_history_stack()
    {
        history_stack.clear();
    }

    Result negamax(Position position, int alpha, int beta, int depth)
    {
        if (depth == 0)
        {
            ++node_count;
            if(was_position_reached(history_stack, position.halfclock()))
                return {STALEMATE_SCORE, {}};
            return {Evaluation::evaluate(position), {}};
        }
        
        const auto &entry = transposition_table.probe(position.hashkey());
        if(!entry && entry->depth >= depth)
        {
            switch (entry->node_type)
            {
            case TTEntry::NodeType::EXACT:
            case TTEntry::NodeType::LOWERBOUND:
                return {entry->score, entry->best_move};
                break;
            case TTEntry::NodeType::UPPERBOUND:
                alpha = entry->score;
            }
        }

        MoveList moves;

        MoveGenerator move_generator_{&position, &moves};
        move_generator_.gen_all_moves();

        MoveOrderer move_orderer{moves, get_hash_move(entry)};

        Result best_result {MATE_SCORE, {}};

        uint32_t legal_move_counter = 0;
        while (move_orderer.has_next())
        {
            Move move = move_orderer.next();
            
            auto hashkey = position.hashkey();
            Position::IrrecoverableState irrecoverable_state = position.irrecoverable_state();
            
            position.make_move(move);
            Search::update_history_stack(position.hashkey(), position.halfclock());

            if (!position.is_in_check(toggle_color(position.side_to_move())))
            {
                ++legal_move_counter;
                auto result = -negamax(position, -beta, -alpha, depth - 1);
                result.move = move;
                if (result.score > best_result.score)
                {
                    best_result = result;
                    if( result.score > alpha )
                    {
                        TTEntry tt_entry = {.key = hashkey, 
                            .best_move = best_result.move, 
                            .depth = depth,
                            .score = best_result.score,
                            .node_type = TTEntry::NodeType::UPPERBOUND};
                        transposition_table.insert(std::move(tt_entry));
                        alpha = result.score;
                    }
                }
                if(result.score >= beta)
                {
                    TTEntry tt_entry = {.key = hashkey, 
                            .best_move = result.move, 
                            .depth = depth,
                            .score = result.score,
                            .node_type = TTEntry::NodeType::LOWERBOUND};
                    transposition_table.insert(std::move(tt_entry));
                    return best_result;
                }
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
            node_count = 0;
            result = negamax(position, MATE_SCORE, -MATE_SCORE, depth);
            std::cout << std::format("[Debug] Depth: {}, Move: {}, Score: {}, Node Count: {:L}", depth, result.move.uci_notation(), result.score, node_count) << std::endl;
        }
        return result.move;
    }
}