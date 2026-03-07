#include "gtest/gtest.h"
#include "bitboard.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "position.hpp"
#include "perft.hpp"

TEST(perft, standard)
{
    // TODO: move init functions to global setup
    Rays::init();
    Patterns::init();

    Position position;

    EXPECT_EQ(Perft::test_copy(position, 1), 20);
    EXPECT_EQ(Perft::test_copy(position, 2), 400);
    EXPECT_EQ(Perft::test_copy(position, 3), 8902);
    EXPECT_EQ(Perft::test_copy(position, 4), 197281);
    EXPECT_EQ(Perft::test_copy(position, 5), 4865609);
    EXPECT_EQ(Perft::test_copy(position, 6), 119060324);
}

TEST(perft, kiwipete)
{
    Rays::init();
    Patterns::init();

    Position position{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};

    EXPECT_EQ(Perft::test_copy(position, 1), 48);
    EXPECT_EQ(Perft::test_copy(position, 2), 2039);
    EXPECT_EQ(Perft::test_copy(position, 3), 97862);
    EXPECT_EQ(Perft::test_copy(position, 4), 4085603);
    EXPECT_EQ(Perft::test_copy(position, 5), 193690690);
}

TEST(perft, endgame)
{
    Rays::init();
    Patterns::init();

    Position position{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"};

    EXPECT_EQ(Perft::test_copy(position, 1), 14);
    EXPECT_EQ(Perft::test_copy(position, 2), 191);
    EXPECT_EQ(Perft::test_copy(position, 3), 2812);
    EXPECT_EQ(Perft::test_copy(position, 4), 43238);
    EXPECT_EQ(Perft::test_copy(position, 5), 674624);
    EXPECT_EQ(Perft::test_copy(position, 6), 11030083);
    EXPECT_EQ(Perft::test_copy(position, 7), 178633661);
}

TEST(perft, en_passants)
{
    Rays::init();
    Patterns::init();

    Position position{"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"};

    EXPECT_EQ(Perft::test_copy(position, 1), 6);
    EXPECT_EQ(Perft::test_copy(position, 2), 264);
    EXPECT_EQ(Perft::test_copy(position, 3), 9467);
    EXPECT_EQ(Perft::test_copy(position, 4), 422333);
    EXPECT_EQ(Perft::test_copy(position, 5), 15833292);
}

TEST(perft, various)
{
    Rays::init();
    Patterns::init();

    Position position{"r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"};
    EXPECT_EQ(Perft::test_copy(position, 1), 8);

    position = Position{"8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"};
    EXPECT_EQ(Perft::test_copy(position, 1), 8);

    position = Position{"r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"};
    EXPECT_EQ(Perft::test_copy(position, 1), 19);

    position = Position{"r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2"};
    EXPECT_EQ(Perft::test_copy(position, 1), 5);

    position = Position{"2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2"};
    EXPECT_EQ(Perft::test_copy(position, 1), 44);

    position = Position{"rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"};
    EXPECT_EQ(Perft::test_copy(position, 1), 39);

    position = Position{"2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"};
    EXPECT_EQ(Perft::test_copy(position, 1), 9);

    position = Position{"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"};
    EXPECT_EQ(Perft::test_copy(position, 3), 62379);

    position = Position{"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"};
    EXPECT_EQ(Perft::test_copy(position, 3), 89890);

    position = Position{"3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 1134888);

    position = Position{"8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 1015133);

    position = Position{"8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 1440467);

    position = Position{"5k2/8/8/8/8/8/8/4K2R w K - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 661072);

    position = Position{"3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 803711);

    position = Position{"r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 4), 1274206);

    position = Position{"r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 4), 1720476);

    position = Position{"2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 3821001);

    position = Position{"8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 5), 1004658);

    position = Position{"4k3/1P6/8/8/8/8/K7/8 w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 217342);

    position = Position{"8/P1k5/K7/8/8/8/8/8 w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 92683);

    position = Position{"K1k5/8/P7/8/8/8/8/8 w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 6), 2217);

    position = Position{"8/k1P5/8/1K6/8/8/8/8 w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 7), 567584);

    position = Position{"8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 4), 23527);
}

TEST(perft, quirky)
{
    Position position = Position{"r5rk/5p1p/5R2/4B3/8/8/7P/7K w - - 0 1"};
    EXPECT_EQ(Perft::test_copy(position, 1), 24);
}