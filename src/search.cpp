#include "search.hpp"
#include "movegenerator.hpp"
#include "moveorderer.hpp"
#include "eval.hpp"
#include "transpositiontable.hpp"
#include "timer.hpp"

#include <boost/container/small_vector.hpp>

#include <limits>
#include <iostream>
#include <format>

using HistoryStack = boost::container::small_vector<Zobrist::HashKey, 256>;

namespace Search
{
    constexpr uint8_t SEARCH_DEPTH = 16;
    constexpr uint8_t MIN_DEPTH = 8;
    constexpr int32_t MAX_PLY = 100;

    constexpr int32_t MATE_SCORE = 15000;
    constexpr int32_t STALEMATE_SCORE = 0;
    constexpr int32_t REPETITION_SCORE = -100;

    HistoryStack history_stack;

    TranspostionTable transposition_table{4_M};

    Killers killers;

    struct Result
    {
        int score;
        Move move;
        Result operator-() {return { -score, move };}
    };

    struct
    {
        uint64_t node_count = 0;
        uint64_t qnode_count = 0;
        uint64_t tt_hits = 0;
        uint64_t threefold_count = 0;
        void clear()
        {
            node_count = 0;
            qnode_count = 0;
            tt_hits = 0;
            threefold_count = 0;
        }
    } search_stats;

    Move get_hash_move(const std::optional<TTEntry> &entry)
    {   
        if(!entry)
            return {};
        return entry->best_move;
    }

    bool was_threefold_reached(const HistoryStack& history_stack)
    {
        auto current_position = history_stack.back();
        auto cnt = 0u;
        for(int i = history_stack.size() - 2; i >= 0; --i)
        {
            if(history_stack[i] == current_position)
                ++cnt;
            if(cnt == 2)
                return true;
        }
        return false;
    }

    int32_t to_tt_score(int32_t score, int sply)
    {
        if(score >= (MATE_SCORE - MAX_PLY))
            return score + sply;
        if(score <= -(MATE_SCORE - MAX_PLY))
            return score - sply;
        return score;
    }

    int32_t from_tt_value(int32_t score, int sply)
    {
        if(score >= (MATE_SCORE - MAX_PLY))
            return score - sply;
        if(score <= -(MATE_SCORE - MAX_PLY))
            return score + sply;
        return score;
    }

    void push_to_history_stack(Zobrist::HashKey key)
    {
            history_stack.push_back(key);
    }
    
    void pop_from_history_stack()
    {
        history_stack.pop_back();
    }

    void clear_history_stack()
    {
        history_stack.clear();
    }

    int32_t quiescence(Position& position, int alpha, int beta)
    {
        ++search_stats.qnode_count;

        auto best_score = Evaluation::evaluate(position);
        if(best_score >= beta)
            return best_score;
        if(best_score > alpha)
            alpha = best_score;

        MoveList moves;
        MoveGenerator move_generator_{&position, &moves};
        move_generator_.gen_tactical_moves();

        MoveOrderer move_orderer{moves, {}};

        while (move_orderer.has_next())
        {
            Move move = move_orderer.next();

            auto irrecoverable_state = position.irrecoverable_state();

            position.make_move(move);

            if (!position.is_in_check(toggle_color(position.side_to_move())))
            {
                auto score = -quiescence(position, -beta, -alpha);

                if(score > best_score)
                    best_score = score;
                if(best_score > alpha)
                    alpha = best_score;
                if(best_score >= beta)
                {
                    position.unmake_move(move, irrecoverable_state);
                    return best_score;
                }
            }
            position.unmake_move(move, irrecoverable_state);
        }
        return best_score;
    }

    Result negamax(Position& position, int alpha, int beta, int depth, int sply)
    {
        if(was_threefold_reached(history_stack))
        {
            +search_stats.threefold_count;
            return {STALEMATE_SCORE, {}};
        }
        if (depth == 0)
        {
            ++search_stats.node_count;
            return {quiescence(position, alpha, beta), {}};
        }
        
        const auto &entry = transposition_table.probe(position.hashkey());
        if(entry && entry->depth >= depth)
        {
            ++search_stats.tt_hits;
            auto adjusted_score = from_tt_value(entry->score, sply);
            switch (entry->node_type)
            {
            case TTEntry::NodeType::EXACT:
                return {adjusted_score, entry->best_move};
            case TTEntry::NodeType::LOWERBOUND:
                alpha = std::max(alpha, adjusted_score);
                break;
            case TTEntry::NodeType::UPPERBOUND:
                beta = std::min(beta, adjusted_score);
                break;
            }
            if (alpha >= beta) 
                return {adjusted_score, entry->best_move};
        }

        MoveList moves;

        MoveGenerator move_generator_{&position, &moves};
        move_generator_.gen_all_moves();

        MoveOrderer move_orderer{moves, killers[sply], get_hash_move(entry)};

        Result best_result {-MATE_SCORE, {}};

        uint32_t legal_move_counter = 0;
        
        auto alpha_initial = alpha;

        while (move_orderer.has_next())
        {
            Move move = move_orderer.next();
            
            auto hashkey = position.hashkey();
            auto irrecoverable_state = position.irrecoverable_state();
            
            position.make_move(move);
            push_to_history_stack(position.hashkey());

            if (!position.is_in_check(toggle_color(position.side_to_move())))
            {
                ++legal_move_counter;
                auto result = -negamax(position, -beta, -alpha, depth - 1, sply + 1);
                result.move = move;
                if (result.score > best_result.score)
                {
                    best_result = result;
                    if( result.score > alpha )
                        alpha = result.score;
                }
                if(result.score >= beta)
                {
                    if(result.move.is_quiet())
                    {
                        std::swap(killers[sply].first, killers[sply].second);
                        killers[sply].first = result.move;
                    } 
                    TTEntry tt_entry = {.key = hashkey, 
                            .best_move = result.move, 
                            .depth = depth,
                            .score = to_tt_score(result.score, sply),
                            .node_type = TTEntry::NodeType::LOWERBOUND};
                    transposition_table.insert(std::move(tt_entry));
                    position.unmake_move(move, irrecoverable_state);
                    pop_from_history_stack();
                    return best_result;
                }
            }

            position.unmake_move(move, irrecoverable_state);
            pop_from_history_stack();
        }

        if (legal_move_counter == 0)
        {
            if (position.is_in_check(position.side_to_move()))
                best_result.score = -MATE_SCORE - sply;
            else
                best_result.score = STALEMATE_SCORE;
        }
        else if(best_result.move == Move{})
        {
            best_result.move = moves.front();
        }

        TTEntry::NodeType type = (best_result.score > alpha_initial) ? TTEntry::NodeType::EXACT : TTEntry::NodeType::UPPERBOUND;
        TTEntry tt_entry = {.key = position.hashkey(), 
                            .best_move = best_result.move, 
                            .depth = depth,
                            .score = to_tt_score(best_result.score, sply),
                            .node_type = type};
        transposition_table.insert(std::move(tt_entry));

        return best_result;
    }

    Move iterative_deepening_search(Position &position)
    {
        Result result;
        for(unsigned depth = 1; depth <= SEARCH_DEPTH; ++depth)
        {
            search_stats.clear();
            Timer timer;
            timer.start();

            result = negamax(position, -MATE_SCORE, MATE_SCORE, depth, 0);

            timer.stop();

            double speed = double(search_stats.qnode_count)/(timer.duration().count() * 1000);
            std::cout << std::format("[Debug] Depth: {}, Move: {}, Score: {}, Node "
                                 "Count: {:L}, QNode Count: {:L}, TT Hits: {:L}, "
                                 "Threefolds: {}, Speed: {} MNPS",
                                 depth, result.move.uci_notation(),
                                 result.score, search_stats.node_count,
                                 search_stats.qnode_count, search_stats.tt_hits,
                                 search_stats.threefold_count, speed)
                                  << std::endl;
        }
        return result.move;
    }
}