#include <iostream>
#include <vector>

#include "bitboard.hpp"
#include "position.hpp"
#include "moveparser.hpp"

int main()
{
    std::cout << "This is Altair Chess Engine" << std::endl;

    Position position;   
    std::cout << "Starting position: " << std::endl << position.pretty() << std::endl;

    CoordinateMoveParser move_parser(position);
    std::string game_str = "d2d4 g8f6 c2c4 c7c5 d4d5 e7e6 b1c3 e6d5 c4d5 d7d6 g1f3 g7g6 c1g5 f8g7 f3d2 h7h6 g5h4 g6g5 h4g3 f6h5 d2c4 h5g3 h2g3 e8g8 e2e3 d8e7 f1e2 f8d8 e1g1 b8d7 a2a4 d7e5 c4e5 e7e5 a4a5 a8b8 a1a2 c8d7 c3b5 d7b5 e2b5 b7b6 a5a6 b8c8 d1d3 c8c7 b2b3 e5c3 d3c3 g7c3 a2c2 c3f6 g3g4 c7e7 c2c4 d8c8 g2g3 f6g7 f1d1 c8f8 d1d3 g8h7 g1g2 h7g6 d3d1 h6h5 g4h5 g6h5 g3g4 h5g6 c4c2 f8h8 b5d3 g6f6 g2g3 e7e8 d3b5 e8e4 c2c4 e4c4 b3c4 f6e7 b5a4 g7e5 g3f3 h8h4 d1g1 f7f5";
    auto game_sequence = move_parser.parse_sequence(game_str);

    for(auto move : game_sequence)
    {
        position.make_move(move);
        std::cout << "Position after move#" << position.moveclock() << std::endl << position.pretty() << std::endl;
        std::cout << "Fen: " << position.fen() << std::endl;
    }

    return 0;
}