#include "bitboard.hpp"
#include <iostream>

std::string BitBoards::prettify(BitBoard bit_board)
{
    std::string pretty_BB = "----------";

    for(Rank rank = RANK_1; rank < N_RANKS; ++rank)
    {
        pretty_BB += '\n';
        pretty_BB += std::to_string(8 - rank);
        pretty_BB += ' ';

        for(File file_ind = FILE_A; file_ind < N_FILES; ++file_ind)
        {
            if(bit_board & (1ULL << (64 - (8 - file_ind))))
                pretty_BB += 'x';
            else
                pretty_BB += '.';
        }

        bit_board <<= 8;
    }

    pretty_BB += "\n  ABCDEFGH \n";

    return pretty_BB;
}