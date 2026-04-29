#include "search.hpp"
#include "movegenerator.hpp"
#include "moveorderer.hpp"
#include "eval.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <limits>


constexpr uint8_t SEARCH_DEPTH = 22;
constexpr uint8_t MIN_DEPTH = 6;
constexpr int32_t MAX_PLY = 100;

constexpr int32_t MATE_SCORE = 15000;
constexpr int32_t STALEMATE_SCORE = 0;
constexpr int32_t REPETITION_SCORE = -100;

constexpr int32_t NODE_STEP = 2047;

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

bool was_threefold_reached(const Search::HistoryStack& history_stack)
{
    auto current_position = history_stack.entries.back();
    for(int i = history_stack.entries.size() - 2; i >= 0; --i)
    {
        if(history_stack.entries[i] == current_position)
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

void update_killers(Killers& killers, int sply, Move move)
{
    if(killers[sply].first != move)
    {
        std::swap(killers[sply].first, killers[sply].second);
        killers[sply].first = move;
    }
}

void update_history_scores(int32_t history_scores[N_SQUARES][N_SQUARES],
                           int bonus, Move move) 
{
    auto clamped_bonus = std::clamp(bonus, -MAX_HISTORY_SCORE, MAX_HISTORY_SCORE);
    history_scores[move.from()][move.to()] += 
        clamped_bonus - history_scores[move.from()][move.to()] * abs(clamped_bonus) / MAX_HISTORY_SCORE;
}

int LMR(int depth, int move_index)
{
    if(depth > 3)
    {
        return 0.2 + std::log(depth) / 2 + std::log(move_index) / 5;
    }
    return 0;
}

void Search::HistoryStack::push(Zobrist::HashKey key) { entries.push_back(key); }
void Search::HistoryStack::pop() { entries.pop_back(); }
void Search::HistoryStack::clear() { entries.clear(); }

void Search::reset()
{
    clear();
    history_stack.clear();
    transposition_table_.clear();
}

void Search::setup_time_manager(std::unique_ptr<TimeManager> time_manager)
{
    time_manager_ = std::move(time_manager);
}

void Search::clear()
{
    std::ranges::fill(killers_, std::pair{Move{}, Move{}});
    memset(history_scores_, 0, sizeof(history_scores_));
}

int32_t Search::quiescence(Position& position, int alpha, int beta)
{
    if(search_stats.qnode_count % NODE_STEP == 0 && time_manager_->is_over())
        return 0;
    ++search_stats.qnode_count;

    auto best_score = Evaluation::evaluate(position);
    if(best_score >= beta)
        return best_score;
    if(best_score > alpha)
        alpha = best_score;

    MoveList moves;
    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_tactical_moves();

    MoveOrderer move_orderer{moves, {}, nullptr};

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

Result Search::negamax(Position& position, int alpha, int beta, int depth, bool can_reduce, int sply)
{
    if(search_stats.qnode_count % NODE_STEP == 0 && time_manager_->is_over())
        return {0, {}};
    if(was_threefold_reached(history_stack) && sply != 0)
    {
        ++search_stats.threefold_count;
        return {STALEMATE_SCORE, {}};
    }
    if (depth == 0)
    {
        ++search_stats.node_count;
        return {quiescence(position, alpha, beta), {}};
    }
        
    const auto &entry = transposition_table_.probe(position.hashkey());
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

    MoveOrderer move_orderer{moves, killers_[sply],
                             history_scores_[position.side_to_move()],
                             get_hash_move(entry)};

    Result best_result {-MATE_SCORE, {}};

    uint32_t legal_move_counter = 0;
        
    auto alpha_initial = alpha;
    auto side_to_move = position.side_to_move();

    while (move_orderer.has_next())
    {
        Move move = move_orderer.next();
            
        auto hashkey = position.hashkey();
        auto irrecoverable_state = position.irrecoverable_state();
            
        position.make_move(move);
        history_stack.push(position.hashkey());

        if (!position.is_in_check(toggle_color(position.side_to_move())))
        {
            ++legal_move_counter;
            Result result;
            if(legal_move_counter != 1)
            {
                int reductions = can_reduce * LMR(depth, legal_move_counter);
                result = -negamax(position, -alpha - 1, -alpha, depth - 1 - reductions, false, sply + 1);
                if(result.score > alpha && result.score < beta)
                {
                    result = -negamax(position, -beta, -alpha, depth - 1 - reductions, false, sply + 1);
                }

            }
            else 
            {
                result = -negamax(position, -beta, -alpha, depth - 1, true, sply + 1);
            } 
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
                    update_killers(killers_, sply, result.move);
                    update_history_scores(history_scores_[side_to_move],
                                          depth * depth, result.move);
                }
                TTEntry tt_entry = {.key = hashkey, 
                        .best_move = result.move, 
                        .depth = depth,
                        .score = to_tt_score(result.score, sply),
                        .node_type = TTEntry::NodeType::LOWERBOUND};
                transposition_table_.insert(tt_entry);
                position.unmake_move(move, irrecoverable_state);
                history_stack.pop();
                return best_result;
            }
        }

        position.unmake_move(move, irrecoverable_state);
        history_stack.pop();
    }

    if (legal_move_counter == 0)
    {
        if (position.is_in_check(position.side_to_move()))
            best_result.score = -MATE_SCORE - sply;
        else
            best_result.score = STALEMATE_SCORE;
    }
    else if(best_result.move.is_null())
    {
        best_result.move = moves.front();
    }

    if(search_stats.qnode_count % NODE_STEP == 0 && time_manager_->is_over())
        return {0, {}};

    TTEntry::NodeType type = (best_result.score > alpha_initial) ? TTEntry::NodeType::EXACT : TTEntry::NodeType::UPPERBOUND;
    TTEntry tt_entry = {.key = position.hashkey(), 
                        .best_move = best_result.move, 
                        .depth = depth,
                        .score = to_tt_score(best_result.score, sply),
                        .node_type = type};
    transposition_table_.insert(tt_entry);

    return best_result;
}

Move Search::iterative_deepening(Position &position)
{
    clear();
    Result final_result;
    for(unsigned depth = 1; depth <= SEARCH_DEPTH; ++depth)
    {
        search_stats.clear();
        Timer timer;
        timer.start();

        auto result = negamax(position, -MATE_SCORE, MATE_SCORE, depth, true, 0);
        
        timer.stop();
        
        if(!time_manager_->is_over())
            final_result = result;
        else
            break;

        print_info(depth, result, timer);
    }
    return final_result.move;
}

void Search::print_info(unsigned depth, const Result& result, const Timer<>& timer)
{
    double speed = double(search_stats.qnode_count)/(timer.duration().count() * 1000);
    std::cout << std::format(std::locale("en_US.UTF-8"),
                             "[Debug] Depth: {}, Move: {}, Score: {}, Node "
                             "Count: {:L}, QNode Count: {:L}, TT Hits: {:L}, "
                             "Threefolds: {}, Speed: {} MNPS",
                             depth, result.move.uci_notation(), result.score,
                             search_stats.node_count, search_stats.qnode_count,
                             search_stats.tt_hits, search_stats.threefold_count,
                             speed)
                              << std::endl;

    std::cout << std::format("info depth {} score cp {} nodes {} time {} nps "
                             "{} currmove {} tbhits {}",
                             depth, result.score, search_stats.qnode_count,
                             static_cast<int>(timer.duration().count()),
                             static_cast<int>(speed * 1000 * 1000),
                             result.move.uci_notation(), search_stats.tt_hits)
              << std::endl;
}