#include <iostream>
#include <vector>
#include <chrono>

#include "bitboard.hpp"
#include "position.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "perft.hpp"
#include "movegenerator.hpp"
#include "moveparser.hpp"

int main(int argc, char *argv[])
{
    Rays::init();
    Patterns::init();

    if (argc != 3)
    {
        throw std::runtime_error("Invalid number of arguments");
    }

    int depth = std::stoi(argv[1]);
    std::string postition_str = argv[2];

    Position position{postition_str};

    std::vector<Move> moves;
    moves.reserve(256);

    Position::IrrecoverableState irrecoverable_state;

    MoveGenerator move_generator_{&position, &moves};
    move_generator_.gen_all_moves();
    uint64_t node_count = 0;
    for (auto move : moves)
    {
        irrecoverable_state = position.irrecoverable_state();
        position.make_move(move);
        if (!position.is_in_check(toggle_color(position.side_to_move())))
        {
            uint64_t curr_node_count = Perft::test(position, depth - 1);
            node_count += curr_node_count;
            std::cout << move.uci_notation() << " " << curr_node_count << std::endl;
        }
        position.unmake_move(move, irrecoverable_state);
    }

    std::cout << std::endl
              << node_count << std::endl;

    return 0;
}