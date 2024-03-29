#include "gtest/gtest.h"
#include "utils.hpp"
#include "defs.hpp"

TEST(lowlevel_utils, popcount) 
{
    EXPECT_EQ(popcount(0x0000000000000000ULL), 0);
    EXPECT_EQ(popcount(0xFFFFFFFFFFFFFFFFULL), 64);
    EXPECT_EQ(popcount(0x55AA55AA55AA55AAULL), 32);
    EXPECT_EQ(popcount(0xAA55AA55AA55AA55ULL), 32);
    EXPECT_EQ(popcount(0x0000000000000001ULL), 1);
    EXPECT_EQ(popcount(0x1000000000000000ULL), 1);
}

TEST(lowlevel_utils, LSB) 
{
    EXPECT_EQ(LSB(0x0000000000000000ULL), 64);
    EXPECT_EQ(LSB(0xFFFFFFFFFFFFFFFFULL), 0);
    EXPECT_EQ(LSB(0x55AA55AA55AA55AAULL), 1);
    EXPECT_EQ(LSB(0xAA55AA55AA55AA55ULL), 0);
    EXPECT_EQ(LSB(0x0000000000000001ULL), 0);
    EXPECT_EQ(LSB(0x1000000000000000ULL), 60);
    EXPECT_EQ(LSB(0xF0F0F0F0F0F0F040ULL), 6);
    EXPECT_EQ(LSB(0xCF1234FFCFFF8000ULL), 15);
}

TEST(lowlevel_utils, pop_LSB) 
{
    uint64_t test_ll = 0x0000000000000000ULL;
    EXPECT_EQ(pop_LSB(test_ll), 64);
    EXPECT_EQ(test_ll, 0x0000000000000000ULL);

    test_ll = 0xFFFFFFFFFFFFFFFFULL;
    EXPECT_EQ(pop_LSB(test_ll), 0);
    EXPECT_EQ(test_ll, 0xFFFFFFFFFFFFFFFEULL);
    
    test_ll = 0x55AA55AA55AA55AAULL;
    EXPECT_EQ(pop_LSB(test_ll), 1);
    EXPECT_EQ(test_ll, 0x55AA55AA55AA55A8ULL);
    
    test_ll = 0xAA55AA55AA55AA55ULL;
    EXPECT_EQ(pop_LSB(test_ll), 0);
    EXPECT_EQ(test_ll, 0xAA55AA55AA55AA54ULL);
    
    test_ll = 0x0000000000000001ULL;
    EXPECT_EQ(pop_LSB(test_ll), 0);
    EXPECT_EQ(test_ll, 0x0000000000000000ULL);
    
    test_ll = 0x1000000000000000ULL;
    EXPECT_EQ(pop_LSB(test_ll), 60);
    EXPECT_EQ(test_ll, 0x0000000000000000ULL);
    
    test_ll = 0xF0F0F0F0F0F0F040ULL;
    EXPECT_EQ(pop_LSB(test_ll), 6);
    EXPECT_EQ(test_ll, 0xF0F0F0F0F0F0F000ULL);
    
    test_ll = 0xCF1234FFCFFF8000ULL;
    EXPECT_EQ(pop_LSB(test_ll), 15);
    EXPECT_EQ(test_ll, 0xCF1234FFCFFF0000ULL);
}

TEST(directions, north) 
{
    EXPECT_EQ(step<NORTH>(SQ_A1), SQ_A2);
    EXPECT_EQ(step<NORTH>(SQ_B3), SQ_B4);
    EXPECT_EQ(step<NORTH>(SQ_C6), SQ_C7);
    EXPECT_EQ(step<NORTH>(SQ_D2), SQ_D3);
}

TEST(directions, north_stepped) 
{
    EXPECT_EQ(step<NORTH>(SQ_A1, 1), SQ_A2);
    EXPECT_EQ(step<NORTH>(SQ_B3, 2), SQ_B5);
    EXPECT_EQ(step<NORTH>(SQ_C4, 3), SQ_C7);
    EXPECT_EQ(step<NORTH>(SQ_D2, 4), SQ_D6);
}

TEST(directions, north_west) 
{
    EXPECT_EQ(step<NORTH_WEST>(SQ_H5), SQ_G6);
    EXPECT_EQ(step<NORTH_WEST>(SQ_B3), SQ_A4);
    EXPECT_EQ(step<NORTH_WEST>(SQ_C6), SQ_B7);
    EXPECT_EQ(step<NORTH_WEST>(SQ_D2), SQ_C3);
}

TEST(directions, west) 
{
    EXPECT_EQ(step<WEST>(SQ_H1), SQ_G1);
    EXPECT_EQ(step<WEST>(SQ_B3), SQ_A3);
    EXPECT_EQ(step<WEST>(SQ_C6), SQ_B6);
    EXPECT_EQ(step<WEST>(SQ_D2), SQ_C2);
}

TEST(directions, south_west) 
{
    EXPECT_EQ(step<SOUTH_WEST>(SQ_H5), SQ_G4);
    EXPECT_EQ(step<SOUTH_WEST>(SQ_B3), SQ_A2);
    EXPECT_EQ(step<SOUTH_WEST>(SQ_C6), SQ_B5);
    EXPECT_EQ(step<SOUTH_WEST>(SQ_D2), SQ_C1);
}

TEST(directions, south) 
{
    EXPECT_EQ(step<SOUTH>(SQ_A5), SQ_A4);
    EXPECT_EQ(step<SOUTH>(SQ_B3), SQ_B2);
    EXPECT_EQ(step<SOUTH>(SQ_C6), SQ_C5);
    EXPECT_EQ(step<SOUTH>(SQ_D2), SQ_D1);
}

TEST(directions, south_east) 
{
    EXPECT_EQ(step<SOUTH_EAST>(SQ_A5), SQ_B4);
    EXPECT_EQ(step<SOUTH_EAST>(SQ_B3), SQ_C2);
    EXPECT_EQ(step<SOUTH_EAST>(SQ_C6), SQ_D5);
    EXPECT_EQ(step<SOUTH_EAST>(SQ_D2), SQ_E1);
}

TEST(directions, east)
{
    EXPECT_EQ(step<EAST>(SQ_A5), SQ_B5);
    EXPECT_EQ(step<EAST>(SQ_B3), SQ_C3);
    EXPECT_EQ(step<EAST>(SQ_C6), SQ_D6);
    EXPECT_EQ(step<EAST>(SQ_D2), SQ_E2);
}

TEST(directions, north_east) 
{
    EXPECT_EQ(step<NORTH_EAST>(SQ_A3), SQ_B4);
    EXPECT_EQ(step<NORTH_EAST>(SQ_B5), SQ_C6);
    EXPECT_EQ(step<NORTH_EAST>(SQ_C6), SQ_D7);
    EXPECT_EQ(step<NORTH_EAST>(SQ_D2), SQ_E3);
}


TEST(conversions, get_file)
{
    EXPECT_EQ(get_file(SQ_A4), FILE_A);
    EXPECT_EQ(get_file(SQ_C1), FILE_C);
    EXPECT_EQ(get_file(SQ_H3), FILE_H);
    EXPECT_EQ(get_file(SQ_D4), FILE_D);
    EXPECT_EQ(get_file(SQ_E2), FILE_E);
    EXPECT_EQ(get_file(SQ_B4), FILE_B);
    EXPECT_EQ(get_file(SQ_G1), FILE_G);
    EXPECT_EQ(get_file(SQ_B2), FILE_B);
    EXPECT_EQ(get_file(SQ_A5), FILE_A);
    EXPECT_EQ(get_file(SQ_H6), FILE_H);
    EXPECT_EQ(get_file(SQ_D3), FILE_D);
    EXPECT_EQ(get_file(SQ_G2), FILE_G);
    EXPECT_EQ(get_file(SQ_F7), FILE_F);
    EXPECT_EQ(get_file(SQ_B3), FILE_B);
    EXPECT_EQ(get_file(SQ_D8), FILE_D);
    EXPECT_EQ(get_file(SQ_E7), FILE_E);
}

TEST(conversions, get_rank)
{
    EXPECT_EQ(get_rank(SQ_A4), RANK_4);
    EXPECT_EQ(get_rank(SQ_C1), RANK_1);
    EXPECT_EQ(get_rank(SQ_H3), RANK_3);
    EXPECT_EQ(get_rank(SQ_D4), RANK_4);
    EXPECT_EQ(get_rank(SQ_E2), RANK_2);
    EXPECT_EQ(get_rank(SQ_B4), RANK_4);
    EXPECT_EQ(get_rank(SQ_G1), RANK_1);
    EXPECT_EQ(get_rank(SQ_B2), RANK_2);
    EXPECT_EQ(get_rank(SQ_A5), RANK_5);
    EXPECT_EQ(get_rank(SQ_H6), RANK_6);
    EXPECT_EQ(get_rank(SQ_D3), RANK_3);
    EXPECT_EQ(get_rank(SQ_G2), RANK_2);
    EXPECT_EQ(get_rank(SQ_F7), RANK_7);
    EXPECT_EQ(get_rank(SQ_B3), RANK_3);
    EXPECT_EQ(get_rank(SQ_D8), RANK_8);
    EXPECT_EQ(get_rank(SQ_E7), RANK_7);
}

TEST(conversions, get_color)
{
    EXPECT_EQ(get_color(WH_PAWN)     , WHITE);
    EXPECT_EQ(get_color(BL_PAWN)     , BLACK);
    EXPECT_EQ(get_color(WH_KNIGHT)   , WHITE);
    EXPECT_EQ(get_color(BL_KNIGHT)   , BLACK);
    EXPECT_EQ(get_color(WH_BISHOP)   , WHITE);
    EXPECT_EQ(get_color(BL_BISHOP)   , BLACK);
    EXPECT_EQ(get_color(WH_ROOK)     , WHITE);
    EXPECT_EQ(get_color(BL_ROOK)     , BLACK);
    EXPECT_EQ(get_color(WH_QUEEN)    , WHITE);
    EXPECT_EQ(get_color(BL_QUEEN)    , BLACK);
    EXPECT_EQ(get_color(WH_KING)     , WHITE);
    EXPECT_EQ(get_color(BL_KING)     , BLACK);   
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


TEST(castling_rights, castling_rights)
{
    CastlingRights rights;

    EXPECT_EQ(rights.queen_side(WHITE), true);
    EXPECT_EQ(rights.queen_side(BLACK), true);
    EXPECT_EQ(rights.king_side(WHITE), true);
    EXPECT_EQ(rights.king_side(BLACK), true);

    rights.clear_queen_side(WHITE);
    EXPECT_EQ(rights.queen_side(WHITE), false);

    rights.clear_king_side(BLACK);
    EXPECT_EQ(rights.king_side(BLACK), false);

    rights.set_king_side(BLACK);
    EXPECT_EQ(rights.king_side(BLACK), true);
}

TEST(square_in_between, square_in_between)
{
    EXPECT_EQ(square_in_between(SQ_A1, SQ_A3), SQ_A2);
    EXPECT_EQ(square_in_between(SQ_B4, SQ_B6), SQ_B5);

    EXPECT_EQ(square_in_between(SQ_A3, SQ_A1), SQ_A2);
    EXPECT_EQ(square_in_between(SQ_B6, SQ_B4), SQ_B5);
}

TEST(invert_direction, invert_direction)
{
    EXPECT_EQ(invert_direction(NORTH        ), SOUTH        );
    EXPECT_EQ(invert_direction(NORTH_WEST   ), SOUTH_EAST   );
    EXPECT_EQ(invert_direction(WEST         ), EAST         );
    EXPECT_EQ(invert_direction(SOUTH_WEST   ), NORTH_EAST   );
    EXPECT_EQ(invert_direction(SOUTH        ), NORTH        );
    EXPECT_EQ(invert_direction(SOUTH_EAST   ), NORTH_WEST   );
    EXPECT_EQ(invert_direction(EAST         ), WEST         );
    EXPECT_EQ(invert_direction(NORTH_EAST   ), SOUTH_WEST   );
}