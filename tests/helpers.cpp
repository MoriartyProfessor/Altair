#include "gtest/gtest.h"
#include "defs.hpp"

TEST(directions, north) 
{
    EXPECT_EQ(north(SQ_A1), SQ_A2);
    EXPECT_EQ(north(SQ_B3), SQ_B4);
    EXPECT_EQ(north(SQ_C6), SQ_C7);
    EXPECT_EQ(north(SQ_D2), SQ_D3);
}

TEST(directions, north_west) 
{
    EXPECT_EQ(north_west(SQ_H5), SQ_G6);
    EXPECT_EQ(north_west(SQ_B3), SQ_A4);
    EXPECT_EQ(north_west(SQ_C6), SQ_B7);
    EXPECT_EQ(north_west(SQ_D2), SQ_C3);
}

TEST(directions, west) 
{
    EXPECT_EQ(west(SQ_H1), SQ_G1);
    EXPECT_EQ(west(SQ_B3), SQ_A3);
    EXPECT_EQ(west(SQ_C6), SQ_B6);
    EXPECT_EQ(west(SQ_D2), SQ_C2);
}

TEST(directions, south_west) 
{
    EXPECT_EQ(south_west(SQ_H5), SQ_G4);
    EXPECT_EQ(south_west(SQ_B3), SQ_A2);
    EXPECT_EQ(south_west(SQ_C6), SQ_B5);
    EXPECT_EQ(south_west(SQ_D2), SQ_C1);
}

TEST(directions, south) 
{
    EXPECT_EQ(south(SQ_A5), SQ_A4);
    EXPECT_EQ(south(SQ_B3), SQ_B2);
    EXPECT_EQ(south(SQ_C6), SQ_C5);
    EXPECT_EQ(south(SQ_D2), SQ_D1);
}

TEST(directions, south_east) 
{
    EXPECT_EQ(south_east(SQ_A5), SQ_B4);
    EXPECT_EQ(south_east(SQ_B3), SQ_C2);
    EXPECT_EQ(south_east(SQ_C6), SQ_D5);
    EXPECT_EQ(south_east(SQ_D2), SQ_E1);
}

TEST(directions, east)
{
    EXPECT_EQ(east(SQ_A5), SQ_B5);
    EXPECT_EQ(east(SQ_B3), SQ_C3);
    EXPECT_EQ(east(SQ_C6), SQ_D6);
    EXPECT_EQ(east(SQ_D2), SQ_E2);
}

TEST(directions, north_east) 
{
    EXPECT_EQ(north_east(SQ_A3), SQ_B4);
    EXPECT_EQ(north_east(SQ_B5), SQ_C6);
    EXPECT_EQ(north_east(SQ_C6), SQ_D7);
    EXPECT_EQ(north_east(SQ_D2), SQ_E3);
}


TEST(conversions, make_file)
{
    EXPECT_EQ(make_file(SQ_A4), FILE_A);
    EXPECT_EQ(make_file(SQ_C1), FILE_C);
    EXPECT_EQ(make_file(SQ_H3), FILE_H);
    EXPECT_EQ(make_file(SQ_D4), FILE_D);
    EXPECT_EQ(make_file(SQ_E2), FILE_E);
    EXPECT_EQ(make_file(SQ_B4), FILE_B);
    EXPECT_EQ(make_file(SQ_G1), FILE_G);
    EXPECT_EQ(make_file(SQ_B2), FILE_B);
    EXPECT_EQ(make_file(SQ_A5), FILE_A);
    EXPECT_EQ(make_file(SQ_H6), FILE_H);
    EXPECT_EQ(make_file(SQ_D3), FILE_D);
    EXPECT_EQ(make_file(SQ_G2), FILE_G);
    EXPECT_EQ(make_file(SQ_F7), FILE_F);
    EXPECT_EQ(make_file(SQ_B3), FILE_B);
    EXPECT_EQ(make_file(SQ_D8), FILE_D);
    EXPECT_EQ(make_file(SQ_E7), FILE_E);
}

TEST(conversions, make_rank)
{
    EXPECT_EQ(make_rank(SQ_A4), RANK_4);
    EXPECT_EQ(make_rank(SQ_C1), RANK_1);
    EXPECT_EQ(make_rank(SQ_H3), RANK_3);
    EXPECT_EQ(make_rank(SQ_D4), RANK_4);
    EXPECT_EQ(make_rank(SQ_E2), RANK_2);
    EXPECT_EQ(make_rank(SQ_B4), RANK_4);
    EXPECT_EQ(make_rank(SQ_G1), RANK_1);
    EXPECT_EQ(make_rank(SQ_B2), RANK_2);
    EXPECT_EQ(make_rank(SQ_A5), RANK_5);
    EXPECT_EQ(make_rank(SQ_H6), RANK_6);
    EXPECT_EQ(make_rank(SQ_D3), RANK_3);
    EXPECT_EQ(make_rank(SQ_G2), RANK_2);
    EXPECT_EQ(make_rank(SQ_F7), RANK_7);
    EXPECT_EQ(make_rank(SQ_B3), RANK_3);
    EXPECT_EQ(make_rank(SQ_D8), RANK_8);
    EXPECT_EQ(make_rank(SQ_E7), RANK_7);
}

TEST(conversions, make_square)
{
    EXPECT_EQ(make_square(FILE_A, RANK_4), SQ_A4);
    EXPECT_EQ(make_square(FILE_C, RANK_1), SQ_C1);
    EXPECT_EQ(make_square(FILE_H, RANK_3), SQ_H3);
    EXPECT_EQ(make_square(FILE_D, RANK_4), SQ_D4);
    EXPECT_EQ(make_square(FILE_E, RANK_2), SQ_E2);
    EXPECT_EQ(make_square(FILE_B, RANK_4), SQ_B4);
    EXPECT_EQ(make_square(FILE_G, RANK_1), SQ_G1);
    EXPECT_EQ(make_square(FILE_B, RANK_2), SQ_B2);
    EXPECT_EQ(make_square(FILE_A, RANK_5), SQ_A5);
    EXPECT_EQ(make_square(FILE_H, RANK_6), SQ_H6);
    EXPECT_EQ(make_square(FILE_D, RANK_3), SQ_D3);
    EXPECT_EQ(make_square(FILE_G, RANK_2), SQ_G2);
    EXPECT_EQ(make_square(FILE_F, RANK_7), SQ_F7);
    EXPECT_EQ(make_square(FILE_B, RANK_3), SQ_B3);
    EXPECT_EQ(make_square(FILE_D, RANK_8), SQ_D8);
    EXPECT_EQ(make_square(FILE_E, RANK_7), SQ_E7);
}

TEST(conversions, make_piece)
{
    EXPECT_EQ(make_piece(WHITE, PAWN)   , WH_PAWN);
    EXPECT_EQ(make_piece(WHITE, KNIGHT) , WH_KNIGHT);
    EXPECT_EQ(make_piece(WHITE, BISHOP) , WH_BISHOP);
    EXPECT_EQ(make_piece(WHITE, ROOK)   , WH_ROOK);
    EXPECT_EQ(make_piece(WHITE, QUEEN)  , WH_QUEEN);
    EXPECT_EQ(make_piece(WHITE, KING)   , WH_KING);

    EXPECT_EQ(make_piece(BLACK, PAWN)   , BL_PAWN);
    EXPECT_EQ(make_piece(BLACK, KNIGHT) , BL_KNIGHT);
    EXPECT_EQ(make_piece(BLACK, BISHOP) , BL_BISHOP);
    EXPECT_EQ(make_piece(BLACK, ROOK)   , BL_ROOK);
    EXPECT_EQ(make_piece(BLACK, QUEEN)  , BL_QUEEN);
    EXPECT_EQ(make_piece(BLACK, KING)   , BL_KING);
}