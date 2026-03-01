#include "gtest/gtest.h"

#include "bitboard.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "position.hpp"
#include "zobrist.hpp"

TEST(zobrist, basic) 
{
    Rays::init();
    Patterns::init();
    Zobrist::init();

    Position position;
    Zobrist::HashKey init_key = position.hashkey();

    Move move{SQ_B1, SQ_C3};
    move.set_piece_type(KNIGHT);
    move.set_quiet();
    
    position.make_move(move);
    EXPECT_NE(init_key, position.hashkey());

    move.set_from(SQ_B8);
    move.set_to(SQ_C6);
    position.make_move(move);

    move.set_from(SQ_C3);
    move.set_to(SQ_B1);
    position.make_move(move);

    move.set_from(SQ_C6);
    move.set_to(SQ_B8);
    position.make_move(move);
    EXPECT_EQ(init_key, position.hashkey());
}

TEST(zobrist, unmake) 
{
    Rays::init();
    Patterns::init();
    Zobrist::init();

    Position position;
    Zobrist::HashKey init_key = position.hashkey();

    Move move{SQ_B1, SQ_C3};
    move.set_piece_type(KNIGHT);
    move.set_quiet();

    auto irrecovarable_state = position.irrecoverable_state();

    position.make_move(move);
    EXPECT_NE(init_key, position.hashkey());

    position.unmake_move(move, irrecovarable_state);
    EXPECT_EQ(init_key, position.hashkey());
}