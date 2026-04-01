#include "gtest/gtest.h"

#include "movegenerator.hpp"
#include "moveorderer.hpp"
#include "patterns.hpp"
#include "position.hpp"
#include "rays.hpp"

TEST(moveorderer, basic) {
    Rays::init();
    Patterns::init();

    Position position{"r3k2r/p1ppqpb1/bn1Ppnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};

    MoveList moves;

    MoveGenerator move_generator{&position, &moves};
    move_generator.gen_all_moves();

    MoveOrderer move_orderer{moves, {}, nullptr};

    Move first_move = move_orderer.next();
    EXPECT_EQ(first_move.from(), SQ_D6);
    EXPECT_EQ(first_move.to(), SQ_E7);
    EXPECT_EQ(first_move.is_capture(), true);
    EXPECT_EQ(first_move.capture_piece_type(), PieceTypes::QUEEN);

    Move second_move = move_orderer.next();
    EXPECT_EQ(second_move.from(), SQ_E2);
    EXPECT_EQ(second_move.to(), SQ_A6);
    EXPECT_EQ(second_move.is_capture(), true);
    EXPECT_EQ(second_move.capture_piece_type(), PieceTypes::BISHOP);

    Move third_move = move_orderer.next();
    EXPECT_EQ(third_move.from(), SQ_F3);
    EXPECT_EQ(third_move.to(), SQ_F6);
    EXPECT_EQ(third_move.is_capture(), true);
    EXPECT_EQ(third_move.capture_piece_type(), PieceTypes::KNIGHT);
}