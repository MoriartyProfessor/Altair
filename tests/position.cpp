#include "gtest/gtest.h"
#include "bitboard.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "position.hpp"

TEST(position, square_attackers) 
{
    Rays::init();
    Patterns::init();
    
    Position position;

    BitBoard expected_BB = EMPTY_BB;
    EXPECT_EQ(position.square_attackers(SQ_E4), expected_BB);

    expected_BB = BitBoards::from_string((R"(........
                                             ........
                                             ........
                                             ........
                                             ........
                                             ........
                                             x.x.....
                                             x.......)"));
    EXPECT_EQ(position.square_attackers(SQ_B1), expected_BB) << BitBoards::prettify(position.square_attackers(SQ_B1));
}