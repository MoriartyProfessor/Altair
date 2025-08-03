#include "perft.hpp"
#include "defs.hpp"
#include "movegenerator.hpp"

#include "bitboard.hpp"
#include <iostream>

Position::IrrecoverableState irrecoverable_states_[MAX_DEPTH];

uint64_t Perft::test(Position &position, uint32_t depth)
{
    std::vector<Move> moves;
    moves.reserve(256);
    if (depth == 0)
        return 1;

    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_all_moves();
    uint64_t node_count = 0;
    for (auto move : moves)
    {
        irrecoverable_states_[depth] = position.irrecoverable_state();
        position.make_move(move);
        if (!position.is_in_check(toggle_color(position.side_to_move())))
            node_count += test(position, depth - 1);
        position.unmake_move(move, irrecoverable_states_[depth]);
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

    std::vector<Move> moves;
    moves.reserve(256);

    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_all_moves();

    for (auto move : moves)
    {
        irrecoverable_states_[depth] = position.irrecoverable_state();
        position.make_move(move);
        if (!position.is_in_check(toggle_color(position.side_to_move())))
            thorough_test(position, result, move, depth - 1);
        position.unmake_move(move, irrecoverable_states_[depth]);
    }
}