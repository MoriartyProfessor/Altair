#include "perft.hpp"
#include "defs.hpp"
#include "movegenerator.hpp"

MoveGenerator move_generator_;
Position::IrrecoverableState irrecoverable_states_[MAX_DEPTH];

uint64_t Perft::test(Position& position, uint32_t depth)
{
    if(depth == 0)
        return 1;
    uint64_t node_count = 0;
    auto moves = move_generator_.all_moves(position);
    for(auto move : moves)
    {
        irrecoverable_states_[depth] = position.irrecoverable_state();
        position.make_move(move);
        if(!position.is_in_check(toggle_color(position.side_to_move())))
            node_count += test(position, depth - 1);
        position.unmake_move(move, irrecoverable_states_[depth]);
    }
    return node_count;
}