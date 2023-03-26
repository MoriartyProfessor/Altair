#include "bitboard.hpp"
#include "position.hpp"

#include <iostream>

int main()
{
    std::cout << "This is Altair Chess Engine" << std::endl;

    Position position;   
    std::cout << "Starting position: " << std::endl << position.pretty() << std::endl;
    
    std::string side_to_move_str = position.side_to_move() == WHITE ? "White" : "Black";
    
    std::cout << "Side to move: " << side_to_move_str << std::endl;

    std::cout << "Reverse FEN: " << std::endl << position.fen() << std::endl;

    return 0;
}