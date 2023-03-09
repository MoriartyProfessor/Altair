#include "bitboard.hpp"

#include <iostream>

int main()
{
    std::cout << "This is Altair Chess Engine" << std::endl;

    std::cout << "File A Bitboard" << std::endl << BitBoards::prettify(FILE_A_BB);
    std::cout << "File B Bitboard" << std::endl << BitBoards::prettify(FILE_B_BB);
    std::cout << "File C Bitboard" << std::endl << BitBoards::prettify(FILE_C_BB);
    std::cout << "File D Bitboard" << std::endl << BitBoards::prettify(FILE_D_BB);
    std::cout << "File E Bitboard" << std::endl << BitBoards::prettify(FILE_E_BB);
    std::cout << "File F Bitboard" << std::endl << BitBoards::prettify(FILE_F_BB);
    std::cout << "File G Bitboard" << std::endl << BitBoards::prettify(FILE_G_BB);
    std::cout << "File H Bitboard" << std::endl << BitBoards::prettify(FILE_H_BB);

    std::cout << "Rank 1 Bitboard" << std::endl << BitBoards::prettify(RANK_1_BB);
    std::cout << "Rank 2 Bitboard" << std::endl << BitBoards::prettify(RANK_2_BB);
    std::cout << "Rank 3 Bitboard" << std::endl << BitBoards::prettify(RANK_3_BB);
    std::cout << "Rank 4 Bitboard" << std::endl << BitBoards::prettify(RANK_4_BB);
    std::cout << "Rank 5 Bitboard" << std::endl << BitBoards::prettify(RANK_5_BB);
    std::cout << "Rank 6 Bitboard" << std::endl << BitBoards::prettify(RANK_6_BB);
    std::cout << "Rank 7 Bitboard" << std::endl << BitBoards::prettify(RANK_7_BB);
    std::cout << "Rank 8 Bitboard" << std::endl << BitBoards::prettify(RANK_8_BB);

    std::cout << "Light Squares Bitboard" << std::endl << BitBoards::prettify(LIGHT_SQUARES_BB);
    std::cout << "Dark Squares Bitboard" << std::endl << BitBoards::prettify(DARK_SQUARES_BB);


    std::cout << "A1-H8 Diagonal" << std::endl << BitBoards::prettify(0x8040201008040201ULL);
    std::cout << "A1-H8 Antidiagonal" << std::endl << BitBoards::prettify(0x0102040810204080ULL);

    return 0;
}