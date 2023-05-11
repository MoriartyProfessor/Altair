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

int main()
{
    std::cout << "This is Altair Chess Engine" << std::endl;

    Rays::init();
    Patterns::init();

    /*
    for(Square square = SQ_A1; square < N_SQUARES; ++square)
    {
        std::cout << "North "       << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(NORTH        , square)) << std::endl;
        std::cout << "South "       << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(SOUTH        , square)) << std::endl;
        std::cout << "East "        << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(EAST         , square)) << std::endl;
        std::cout << "West "        << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(WEST         , square)) << std::endl;
        std::cout << "North west "  << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(NORTH_WEST   , square)) << std::endl;
        std::cout << "South west "  << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(SOUTH_WEST   , square)) << std::endl;
        std::cout << "South east "  << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(SOUTH_EAST   , square)) << std::endl;
        std::cout << "North east "  << square_to_str(square) << std::endl << BitBoards::prettify(Rays::get(NORTH_EAST   , square)) << std::endl;
    }
    */
   
    // Position position{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};
    Position position;
    std::cout << "Starting position: " << std::endl << position.pretty() << std::endl;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    std::cout << Perft::test(position, 5) << std::endl;
    
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_double.count() << "ms\n";
    

    /*
    Position position{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};
    Position::IrrecoverableState irrecoverable_state;

    std::vector<Move> moves;
    moves.reserve(256);
    
    MoveGenerator move_generator{&position, &moves};
    move_generator.gen_all_moves();
    uint64_t node_count = 0;
    for(auto move : moves)
    {
        irrecoverable_state = position.irrecoverable_state();
        position.make_move(move);
        if(!position.is_in_check(toggle_color(position.side_to_move())))
            std::cout << position.fen() << " " << Perft::test(position, 1) << std::endl;
        position.unmake_move(move, irrecoverable_state);
    }
    */

   /*
    Position position{"rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1"};   
    std::cout << "Starting position: " << std::endl << position.pretty() << std::endl;

    MoveGenerator move_generator;
    auto move_list = move_generator.all_moves(position);

    std::vector<Position::IrrecoverableState> intermediary_states{move_list.size()};

    std::cout << "Move List size: " << move_list.size() << std::endl;

    for(int i = 0; i < move_list.size(); ++i)
    {
        auto irrecoverable_state = position.irrecoverable_state();
        position.make_move(move_list[i]);
        std::cout << "Position after move#" << position.moveclock() << std::endl << position.pretty() << std::endl;
        std::cout << "Fen: " << position.fen() << std::endl;
        position.unmake_move(move_list[i], irrecoverable_state);
    }
    */
    /*
    Position position;   
    std::cout << "Starting position: " << std::endl << position.pretty() << std::endl;

    CoordinateMoveParser move_parser(position);
    std::string game_str = "d2d4 g8f6 c2c4 c7c5 d4d5 e7e6 b1c3 e6d5 c4d5 d7d6 g1f3 g7g6 c1g5 f8g7 f3d2 h7h6 g5h4 g6g5 h4g3 f6h5 d2c4 h5g3 h2g3 e8g8 e2e3 d8e7 f1e2 f8d8 e1g1 b8d7 a2a4 d7e5 c4e5 e7e5 a4a5 a8b8 a1a2 c8d7 c3b5 d7b5 e2b5 b7b6 a5a6 b8c8 d1d3 c8c7 b2b3 e5c3 d3c3 g7c3 a2c2 c3f6 g3g4 c7e7 c2c4 d8c8 g2g3 f6g7 f1d1 c8f8 d1d3 g8h7 g1g2 h7g6 d3d1 h6h5 g4h5 g6h5 g3g4 h5g6 c4c2 f8h8 b5d3 g6f6 g2g3 e7e8 d3b5 e8e4 c2c4 e4c4 b3c4 f6e7 b5a4 g7e5 g3f3 h8h4 d1g1 f7f5";
    auto game_sequence = move_parser.parse_sequence(game_str);

    std::vector<Position::IrrecoverableState> intermediary_states{game_sequence.size()};

    for(int i = 0; i < game_sequence.size(); ++i)
    {
        intermediary_states[i] = position.irrecoverable_state();
        position.make_move(game_sequence[i]);
        std::cout << "Position after move#" << position.moveclock() << std::endl << position.pretty() << std::endl;
        std::cout << "Fen: " << position.fen() << std::endl;
    }

    for(int i = game_sequence.size() - 1; i >= 0; --i)
    {
        position.unmake_move(game_sequence[i], intermediary_states[i]);
        std::cout << "Position after move#" << position.moveclock() << std::endl << position.pretty() << std::endl;
        std::cout << "Fen: " << position.fen() << std::endl;
    }
    */

    return 0;
}