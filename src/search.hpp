#ifndef GWAIHIR_SEARCH_HPP
#define GWAIHIR_SEARCH_HPP

#include "move.hpp"
#include "position.hpp"
#include "timemanager.hpp"
#include "timer.hpp"
#include "transpositiontable.hpp"

#include <boost/container/small_vector.hpp>

#include <memory>

using Killers = std::array<std::pair<Move, Move>, 256>;

class Result;

class Search
{
    public:
    struct HistoryStack
    {
        void push(Zobrist::HashKey key);
        void pop();
        void clear();
        boost::container::small_vector<Zobrist::HashKey, 256> entries;
    } history_stack;
    
    public:
    void clear();
    void setup_time_manager(std::unique_ptr<TimeManager>);
    Move iterative_deepening(Position &position);

    private:
    int32_t quiescence(Position& position, int alpha, int beta);
    Result negamax(Position& position, int alpha, int beta, int depth, int sply);
    

    void print_info(unsigned depth, const Result& result, const Timer<>& timer);
    private:
    Killers killers_;
    TranspostionTable transposition_table_{4_M};
    std::unique_ptr<TimeManager> time_manager_;
};

#endif // GWAIHIR_SEARCH_HPP