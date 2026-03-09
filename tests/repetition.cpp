#include "gtest/gtest.h"
#include "bitboard.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "position.hpp"
#include "search.cpp"

TEST(repetition, basic) 
{
    Rays::init();
    Patterns::init();
    Zobrist::init();

    Position position;
    Search::update_history_stack(position.hashkey(), position.halfclock());

    Move move{SQ_B1, SQ_C3};
    move.set_piece_type(KNIGHT);
    move.set_quiet();
    
    position.make_move(move);
    Search::update_history_stack(position.hashkey(), position.halfclock());

    move.set_from(SQ_B8);
    move.set_to(SQ_C6);
    position.make_move(move);
    Search::update_history_stack(position.hashkey(), position.halfclock());

    EXPECT_EQ(Search::was_threefold_reached(Search::history_stack, position.halfclock()), false);

    move.set_from(SQ_C3);
    move.set_to(SQ_B1);
    position.make_move(move);
    Search::update_history_stack(position.hashkey(), position.halfclock());

    EXPECT_EQ(Search::was_threefold_reached(Search::history_stack, position.halfclock()), false);

    move.set_from(SQ_C6);
    move.set_to(SQ_B8);
    position.make_move(move);
    Search::update_history_stack(position.hashkey(), position.halfclock());
    EXPECT_EQ(Search::was_threefold_reached(Search::history_stack, position.halfclock()), false);
}