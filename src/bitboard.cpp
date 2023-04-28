#include "bitboard.hpp"

std::string BitBoards::prettify(BitBoard bitboard)
{
    std::string pretty_BB = "----------";

    for(Rank rank = RANK_1; rank < N_RANKS; ++rank)
    {
        pretty_BB += '\n';
        pretty_BB += std::to_string(8 - rank);
        pretty_BB += ' ';

        for(File file_ind = FILE_A; file_ind < N_FILES; ++file_ind)
        {
            if(bitboard & (1ULL << (64 - (8 - file_ind))))
                pretty_BB += 'x';
            else
                pretty_BB += '.';
        }

        bitboard <<= 8;
    }

    pretty_BB += "\n\n  ABCDEFGH \n";

    return pretty_BB;
}

BitBoard BitBoards::from_string(const std::string& str)
{
    BitBoard bitboard = EMPTY_BB;
    Square be_square = SQ_A1;
    for(auto ch : str)
    {
        if(ch == 'x')
        {
            Square le_square = make_square(get_file(be_square), N_RANKS - get_rank(be_square) - 1);
            BitBoards::set_square(bitboard, le_square);
        }
        if(ch == '.' || ch == 'x')
        {
            ++be_square;
            if(be_square == N_SQUARES)
                break;
        }
    }
    return bitboard;
}