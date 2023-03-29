#include "gtest/gtest.h"
#include "move.hpp"
#include "moveparser.hpp"

TEST(moveparse, moveparse) 
{
    Position position;
    CoordinateMoveParser move_parser(position);
    Move move = move_parser.parse_move("e2e4");
    EXPECT_EQ(move.is_double_pawn_push(), true);
}