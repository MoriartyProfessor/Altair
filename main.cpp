#include "bitboard.hpp"
#include "position.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "perft.hpp"
#include "movegenerator.hpp"
#include "moveparser.hpp"
#include "search.hpp"
#include "uci.hpp"
#include "zobrist.hpp"
#include "utils.hpp"

#include <iostream>
#include <vector>
#include <chrono>

int main()
{
    UCI::Game game;
    game.start();

    /*Rays::init();
    Patterns::init();
    Zobrist::init();*/
    
    /*Position position{"1r6/r4pkP/2n1p3/1pb2pN1/8/2n5/P4P2/K5RR w - - 0 2"};

    auto best_move = Search::search(position);

    std::cout << best_move.uci_notation() << std::endl;*/

    /*Position position;

    std::cout << "Starting position: " << std::endl
              << position.pretty() << std::endl;

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    Perft::Result perft_result;

    auto t1 = high_resolution_clock::now();

    Perft::thorough_test(position, perft_result, Move{}, 6);

    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "Nodes: " << perft_result.nodes << std::endl;
    std::cout << "Captures: " << perft_result.captures << std::endl;
    std::cout << "En Passants: " << perft_result.en_passants << std::endl;
    std::cout << "Castles: " << perft_result.castles << std::endl;
    std::cout << "Promotions: " << perft_result.promotions << std::endl;
    std::cout << "Checks: " << perft_result.checks << std::endl;

    std::cout << ms_double.count() << "ms\n";

    std::cout << double(perft_result.nodes) / (ms_double.count() * 1000) << " MNPS" << std::endl;*/

    /*Position position{"r5rk/5p1p/5R2/4B3/8/8/7P/7K w - - 0 1"};

    std::cout << "Starting position: " << std::endl
              << position.pretty() << std::endl;

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    auto perft_result = Perft::test_make_unmake(position, 1);

    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << perft_result << " nodes" << std::endl;

    std::cout << ms_double.count() << "ms\n";
    std::cout << double(perft_result) / (ms_double.count() * 1000) << " MNPS" << std::endl;*/

    return 0;
}