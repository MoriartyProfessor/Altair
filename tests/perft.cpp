#include "gtest/gtest.h"
#include "bitboard.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "position.hpp"
#include "perft.hpp"

TEST(perft, en_passants) 
{
    Rays::init();
    Patterns::init();
    
    Position position{"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"};

    EXPECT_EQ(Perft::test(position, 5), 15833292);
}