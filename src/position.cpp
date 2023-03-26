#include <string>
#include <sstream>

#include "position.hpp"
#include "bitboard.hpp"

Position::Position()
{
    set_to_starting();
}

Position::Position(const std::string& fen)
{
    set_from_fen(fen);
}


void Position::set_to_starting()
{
    /* Just use set_from_fen for now, if this functions is invoked often in runtime, maybe replace it with manual setting */
    set_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Position::set_from_fen(const std::string& fen)
{
    auto init_piece_placement = [this] (const std::string& fen_piece_placement)
    {
        clear_piece_bitboards();
        Rank rank = RANK_8;
        File file = FILE_A;
        for(auto ch : fen_piece_placement)
        {
            switch (ch)
            {
                case 'P':
                    BitBoards::set_square(piece_bitboards_[WH_PAWN], make_square(file, rank));
                    break;
                case 'N':
                    BitBoards::set_square(piece_bitboards_[WH_KNIGHT], make_square(file, rank));
                    break;
                case 'B':
                    BitBoards::set_square(piece_bitboards_[WH_BISHOP], make_square(file, rank));
                break;
                case 'R':
                    BitBoards::set_square(piece_bitboards_[WH_ROOK], make_square(file, rank));
                    break;
                case 'Q':
                    BitBoards::set_square(piece_bitboards_[WH_QUEEN], make_square(file, rank));
                    break;
                case 'K':
                    BitBoards::set_square(piece_bitboards_[WH_KING], make_square(file, rank));
                    break;

                case 'p':
                    BitBoards::set_square(piece_bitboards_[BL_PAWN], make_square(file, rank));
                    break;
                case 'n':
                    BitBoards::set_square(piece_bitboards_[BL_KNIGHT], make_square(file, rank));
                    break;
                case 'b':
                    BitBoards::set_square(piece_bitboards_[BL_BISHOP], make_square(file, rank));
                    break;
                case 'r':
                    BitBoards::set_square(piece_bitboards_[BL_ROOK], make_square(file, rank));
                    break;
                case 'q':
                    BitBoards::set_square(piece_bitboards_[BL_QUEEN], make_square(file, rank));
                    break;
                case 'k':
                    BitBoards::set_square(piece_bitboards_[BL_KING], make_square(file, rank));
                    break;

                case '/':
                    --rank;
                    file = FILE_A;
                    continue;
                
                default:
                    file += ch - '0';
                    continue;
            }
            ++file;
        }
    };

    auto init_side_to_move = [this] (const std::string& fen_side_to_move)
    {
        side_to_move_ = fen_side_to_move == "w" ? WHITE : BLACK;
    };

    auto init_castling_rights = [this] (const std::string& fen_castling_rights)
    {
        castling_rights_.clear_all_rights();
        if(fen_castling_rights == "-")
            return;
        for(auto ch : fen_castling_rights)
        {
            switch (ch)
            {
                case 'K':
                    castling_rights_.set_king_side(WHITE);
                    break;
            
                case 'Q':
                    castling_rights_.set_queen_side(WHITE);
                    break;
            
                case 'k':
                    castling_rights_.set_king_side(BLACK);
                    break;
            
                case 'q':
                    castling_rights_.set_queen_side(BLACK);
                    break;
            }
        }
    };

    auto init_en_passant_square = [this] (const std::string& fen_en_passant_square)
    {
        if(fen_en_passant_square == "-")
            en_passant_square_ = N_SQUARES;
        else
        {
            File ep_file = fen_en_passant_square[0] - 'a';
            Rank ep_rank = fen_en_passant_square[1] - '0';     // Should we check if rank lies in the range of 3-6

            en_passant_square_ = make_square(ep_file, ep_rank);
        }
    };

    auto init_halfclock = [this] (const std::string& fen_halfclock)
    {
        halfclock_ = std::stoul(fen_halfclock);
    };

    auto init_moveclock = [this] (const std::string& fen_moveclock)
    {
        moveclock_ = std::stoul(fen_moveclock);
    };

    std::stringstream fen_stream(fen);
    std::string fen_field;

    fen_stream >> fen_field;
    init_piece_placement(fen_field);
    
    fen_stream >> fen_field;
    init_side_to_move(fen_field);
    
    fen_stream >> fen_field;
    init_castling_rights(fen_field);
    
    fen_stream >> fen_field;
    init_en_passant_square(fen_field);
    
    fen_stream >> fen_field;
    init_halfclock(fen_field);
    
    fen_stream >> fen_field;
    init_moveclock(fen_field);
}

std::string Position::fen() const
{
    auto fen_piece_placement = [this]
    {
        auto fen_rank = [this] (Rank rank)
        {
            std::string fen_rank;
            uint32_t gap_counter = 0;
            for(File file = FILE_A; file < N_FILES; ++file)
            {
                Piece occupant_piece = piece_occupying(make_square(file, rank));
                char occupant_piece_ch = piece_to_char(occupant_piece);
                if(occupant_piece_ch == '.')
                {
                    ++gap_counter;
                }
                else
                {
                    if(gap_counter)
                    {
                        fen_rank += std::to_string(gap_counter);
                        gap_counter = 0;
                    }
                    fen_rank += occupant_piece_ch;
                }
            }
            if(gap_counter)
            {
                fen_rank += std::to_string(gap_counter);
                gap_counter = 0;
            }
            return fen_rank;
        };

        std::string fen_piece_placement;

        for(Rank rank = RANK_1; rank < N_RANKS; ++rank)
        {
            fen_piece_placement += fen_rank(7 - rank);
            fen_piece_placement += '/';
        }
        fen_piece_placement.pop_back();
        return fen_piece_placement;
    };

    auto fen_side_to_move = [this]
    {
        std::string fen_side_to_move;
        fen_side_to_move += side_to_move_ == WHITE ? 'w' : 'b';
        return fen_side_to_move;
    };

    auto fen_castling_rights = [this]
    {
        std::string fen_castling_rights;
        if(castling_rights_.king_side(WHITE))
            fen_castling_rights += 'K';
        if(castling_rights_.queen_side(WHITE))
            fen_castling_rights += 'Q';
        if(castling_rights_.king_side(BLACK))
            fen_castling_rights += 'k';
        if(castling_rights_.king_side(BLACK))
            fen_castling_rights += 'q';
        if(fen_castling_rights.empty())
            fen_castling_rights = '-';
        return fen_castling_rights;
    };

    auto fen_en_passant_square = [this]
    {
        std::string fen_en_passant_square;
        if(en_passant_square_ == N_SQUARES)
            fen_en_passant_square = '-';
        else
            fen_en_passant_square = square_to_str(en_passant_square_);
        return fen_en_passant_square;
    };

    auto fen_halfclock = [this]
    {
        return std::to_string(halfclock_);
    };

    auto fen_moveclock = [this]
    {
        return std::to_string(moveclock_);
    };

    std::string fen;
    fen += fen_piece_placement() + ' ';
    fen += fen_side_to_move() + ' ';
    fen += fen_castling_rights() + ' ';
    fen += fen_en_passant_square() + ' ';
    fen += fen_halfclock() + ' ';
    fen += fen_moveclock();

    return fen;
}

std::string Position::pretty() const
{
    std::string pretty_pos = "----------";

    for(Rank rank = RANK_1; rank < N_RANKS; ++rank)
    {
        pretty_pos += '\n';
        pretty_pos += std::to_string(8 - rank);
        pretty_pos += ' ';

        for(File file = FILE_A; file < N_FILES; ++file)
        {
            Piece occupant_piece = piece_occupying(make_square(file, 7 - rank));
            pretty_pos += piece_to_char(occupant_piece);
        }
    }

    pretty_pos += "\n\n  ABCDEFGH \n";

    return pretty_pos;
}

void Position::make_move(Move move)
{
    
}


BitBoard Position::piece_bitboard(Color color, PieceType type) const
{
    return piece_bitboards_[make_piece(color, type)];
}

BitBoard Position::occupancy_bitboard() const
{
    return occupancy_bitboard(WHITE) | occupancy_bitboard(BLACK);
}

BitBoard Position::occupancy_bitboard(Color color) const
{
    BitBoard occupancy_bitboard = EMPTY_BB;
    for(PieceType piece_type = PAWN; piece_type < N_PIECE_TYPES; ++piece_type)
    {
        occupancy_bitboard |= piece_bitboards_[make_piece(color, piece_type)];
    }
    return occupancy_bitboard;
}


Piece Position::piece_occupying(Square square) const
{
    for(Piece piece = WH_PAWN; piece < N_PIECES; ++piece)
    {
        if(BitBoards::is_square_set(piece_bitboards_[piece], square))
            return piece;
    }
    return N_PIECES;
}


Color Position::side_to_move() const
{
    return side_to_move_;
}

CastlingRights Position::castling_rights() const
{
    return castling_rights_;
}

Square Position::en_passant_square() const
{
    return en_passant_square_;
}

uint32_t Position::halfclock() const
{
    return halfclock_;
}

uint32_t Position::moveclock() const
{
    return moveclock_;
}

void Position::clear_piece_bitboards()
{
    for(auto& piece_bitboard : piece_bitboards_)
        piece_bitboard = EMPTY_BB;
}