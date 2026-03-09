#include "perft.hpp"
#include "defs.hpp"
#include "movegenerator.hpp"

#include "bitboard.hpp"
#include <iostream>

uint64_t Perft::test_copy(Position position, uint32_t depth)
{
    MoveList moves;
    if (depth == 0)
        return 1;

    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_all_moves();
    uint64_t node_count = 0;
    for (auto move : moves)
    {
        Position new_pos = position;
        new_pos.make_move(move);
        if (!new_pos.is_in_check(toggle_color(new_pos.side_to_move())))
            node_count += test_copy(new_pos, depth - 1);
    }
    return node_count;
}

uint64_t Perft::test_make_unmake(Position& position, uint32_t depth)
{
    MoveList moves;
    if (depth == 0)
        return 1;

    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_all_moves();
    uint64_t node_count = 0;
    for (auto move : moves)
    {
        auto irrecoverable_state = position.irrecoverable_state();
        position.make_move(move);
        if (!position.is_in_check(toggle_color(position.side_to_move())))
            node_count += test_make_unmake(position, depth - 1);
        position.unmake_move(move, irrecoverable_state);
    }
    return node_count;
}

void Perft::thorough_test(Position &position, Result &result, Move move, uint32_t depth)
{
    if (depth == 0)
    {
        ++result.nodes;
        if (move.is_capture())
            ++result.captures;
        if (move.is_en_passant())
            ++result.en_passants;
        if (move.is_king_side_castle() || move.is_queen_side_castle())
            ++result.castles;
        if (move.is_promotion())
            ++result.promotions;
        if (position.is_in_check(position.side_to_move()))
            ++result.checks;
        return;
    }

    MoveList moves;

    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_all_moves();

    for (auto move : moves)
    {
        auto irrecoverable_state = position.irrecoverable_state();
        position.make_move(move);
        if (!position.is_in_check(toggle_color(position.side_to_move())))
            thorough_test(position, result, move, depth - 1);
        position.unmake_move(move, irrecoverable_state);
    }
}