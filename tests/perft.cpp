#include "gtest/gtest.h"
#include "bitboard.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "position.hpp"
#include "perft.hpp"


TEST(perft, standard) 
{
    Rays::init();
    Patterns::init();
    
    Position position;

    EXPECT_EQ(Perft::test(position, 1), 20);
    EXPECT_EQ(Perft::test(position, 2), 400);
    EXPECT_EQ(Perft::test(position, 3), 8902);
    EXPECT_EQ(Perft::test(position, 4), 197281);
    EXPECT_EQ(Perft::test(position, 5), 4865609);
    EXPECT_EQ(Perft::test(position, 6), 119060324);
}

TEST(perft, kiwipete) {
    Rays::init();
    Patterns::init();
    
    Position position{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};

    EXPECT_EQ(Perft::test(position, 1), 48);
    EXPECT_EQ(Perft::test(position, 2), 2039);
    EXPECT_EQ(Perft::test(position, 3), 97862);
    EXPECT_EQ(Perft::test(position, 4), 4085603);
    EXPECT_EQ(Perft::test(position, 5), 193690690);
}

TEST(perft, endgame) {
    Rays::init();
    Patterns::init();
    
    Position position{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"};

    EXPECT_EQ(Perft::test(position, 1), 14);
    EXPECT_EQ(Perft::test(position, 2), 191);
    EXPECT_EQ(Perft::test(position, 3), 2812);
    EXPECT_EQ(Perft::test(position, 4), 43238);
    EXPECT_EQ(Perft::test(position, 5), 674624);
    EXPECT_EQ(Perft::test(position, 6), 11030083);
    EXPECT_EQ(Perft::test(position, 7), 178633661);
}

TEST(perft, en_passants) 
{
    Rays::init();
    Patterns::init();
    
    Position position{"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"};

    EXPECT_EQ(Perft::test(position, 1), 6);
    EXPECT_EQ(Perft::test(position, 2), 264);
    EXPECT_EQ(Perft::test(position, 3), 9467);
    EXPECT_EQ(Perft::test(position, 4), 422333);
    EXPECT_EQ(Perft::test(position, 5), 15833292);
}
