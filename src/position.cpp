#include <string>
#include <sstream>

#include "position.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "patterns.hpp"

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
    /* Maybe move starting to FEN to seperate constant*/
    set_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Position::set_from_fen(const std::string& fen)
{
    auto init_piece_placement = [this] (const std::string& fen_piece_placement)
    {
        clear_piece_bitboards_();
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
    if(move.is_quite())
        make_quite_move_(move);

    else if(move.is_capture() && move.is_promotion())
        make_capture_promotion_move_(move);

    else if(move.is_capture())
        make_capture_move_(move);

    else if(move.is_promotion())
        make_promotion_move_(move);

    else if(move.is_double_pawn_push())
        make_double_pawn_push_move_(move);

    else if(move.is_king_side_castle())
        make_king_side_castling_move_(move);

    else if(move.is_queen_side_castle())
        make_queen_side_castling_move_(move);

    else if(move.is_en_passant())
        make_en_passant_move_(move);


    if(!castling_rights_.is_all_clear())
        update_castling_rights_in_make_(move);

    update_en_passant_in_make_(move);

    update_halfclock_in_make_(move);
    
    update_moveclock_in_make_(move);

    side_to_move_ = toggle_color(side_to_move_);
}

void Position::unmake_move(Move move, IrrecoverableState irrecoverable_state)
{
    side_to_move_ = toggle_color(side_to_move_);
    
    if(move.is_quite())
        unmake_quite_move_(move);

    else if(move.is_capture() && move.is_promotion())
        unmake_capture_promotion_move_(move);

    else if(move.is_capture())
        unmake_capture_move_(move);

    else if(move.is_promotion())
        unmake_promotion_move_(move);

    else if(move.is_double_pawn_push())
        unmake_double_pawn_push_move_(move);

    else if(move.is_king_side_castle())
        unmake_king_side_castling_move_(move);

    else if(move.is_queen_side_castle())
        unmake_queen_side_castling_move_(move);

    else if(move.is_en_passant())
        unmake_en_passant_move_(move);


    if(!castling_rights_.is_all_set())
        update_castling_rights_in_unmake_(irrecoverable_state.castling_rights);

    update_en_passant_in_unmake_(move);

    update_halfclock_in_unmake_(irrecoverable_state.halfclock);
    
    update_moveclock_in_unmake_(move);
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
    /* Maybe we should keep occupancy bitboards instead of calculating them everytime*/
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

bool Position::is_in_check(Color color) const
{
    Square king_square = LSB(piece_bitboard(color, KING));
    return is_square_attacked(king_square, toggle_color(color));
}

bool Position::is_square_attacked(Square square, Color color) const
{
    return square_attackers(square) & occupancy_bitboard(color);
}

BitBoard Position::square_attackers(Square square) const
{
    BitBoard knights, bishop_queens, rook_queens, kings;
    knights = piece_bitboard(WHITE, KNIGHT) | piece_bitboard(BLACK, KNIGHT);
    bishop_queens = rook_queens = piece_bitboard(WHITE, QUEEN) | piece_bitboard(BLACK, QUEEN);
    bishop_queens |= piece_bitboard(WHITE, BISHOP) | piece_bitboard(BLACK, BISHOP);
    rook_queens |= piece_bitboard(WHITE, ROOK) | piece_bitboard(BLACK, ROOK);
    kings = piece_bitboard(WHITE, KING) | piece_bitboard(BLACK, KING);

    BitBoard attackers = piece_bitboard(WHITE, PAWN) & Patterns::get_pawn_attacks(square, WHITE);
    attackers |= piece_bitboard(BLACK, PAWN) & Patterns::get_pawn_attacks(square, BLACK);
    attackers |= knights & Patterns::get_knight_attacks(square);
    attackers |= bishop_queens & Patterns::get_bishop_attacks(square, occupancy_bitboard());
    attackers |= rook_queens & Patterns::get_rook_attacks(square, occupancy_bitboard());
    attackers |= kings & Patterns::get_king_attacks(square);

    return attackers;
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

Position::IrrecoverableState Position::irrecoverable_state() const
{
    return IrrecoverableState{castling_rights_, halfclock_};
}

Square Position::en_passant_capture_square(Color side_to_move, Square en_passant_square)
{
    if(side_to_move == WHITE)
        return step<SOUTH>(en_passant_square);
    else
        return step<NORTH>(en_passant_square);
}


void Position::clear_piece_bitboards_()
{
    for(auto& piece_bitboard : piece_bitboards_)
        piece_bitboard = EMPTY_BB;
}

void Position::make_quite_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.from(), move.to());
}

void Position::make_capture_move_(Move move)
{
    remove_piece_(make_piece(toggle_color(side_to_move_), move.capture_piece_type()), move.to());
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.from(), move.to());
}

void Position::make_promotion_move_(Move move)
{
    remove_piece_(make_piece(side_to_move_, move.piece_type()), move.from());
    add_piece_(make_piece(side_to_move_, move.promotion_piece_type()), move.to());
}

void Position::make_capture_promotion_move_(Move move)
{
    remove_piece_(make_piece(side_to_move_, move.piece_type()), move.from());
    remove_piece_(make_piece(toggle_color(side_to_move_), move.capture_piece_type()), move.to());
    add_piece_(make_piece(side_to_move_, move.promotion_piece_type()), move.to());
}

void Position::make_double_pawn_push_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.from(), move.to());
}

void Position::make_king_side_castling_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.from(), move.to());

    if(side_to_move_ == WHITE)
        move_piece_(make_piece(side_to_move_, ROOK), SQ_H1, SQ_F1);
    else
        move_piece_(make_piece(side_to_move_, ROOK), SQ_H8, SQ_F8);
}

void Position::make_queen_side_castling_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.from(), move.to());

    if(side_to_move_ == WHITE)
        move_piece_(make_piece(side_to_move_, ROOK), SQ_A1, SQ_D1);
    else
        move_piece_(make_piece(side_to_move_, ROOK), SQ_A8, SQ_D8);
}

void Position::make_en_passant_move_(Move move)
{
    Square capture_square = en_passant_capture_square(side_to_move_, en_passant_square_);

    remove_piece_(make_piece(toggle_color(side_to_move_), PAWN), capture_square);
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.from(), move.to());
}


void Position::update_castling_rights_in_make_(Move move)
{
    if(move.piece_type() == KING)
        castling_rights_.clear_all_color_rights(side_to_move_);
    
    if(move.piece_type() == ROOK)
    {
        if(move.from() == SQ_A1)
            castling_rights_.clear_queen_side(WHITE);
        else if(move.from() == SQ_A8)
            castling_rights_.clear_queen_side(BLACK);
        else if(move.from() == SQ_H1)
            castling_rights_.clear_king_side(WHITE);
        else if(move.from() == SQ_H8)
            castling_rights_.clear_king_side(BLACK);
    }

    /* Maybe it is possible rook move case with capture case, or simplify it in some way*/ 
    if(move.is_capture())
    {
        if(move.to() == SQ_A1)
            castling_rights_.clear_queen_side(WHITE);
        else if(move.to() == SQ_A8)
            castling_rights_.clear_queen_side(BLACK);
        else if(move.to() == SQ_H1)
            castling_rights_.clear_king_side(WHITE);
        else if(move.to() == SQ_H8)
            castling_rights_.clear_king_side(BLACK);
    }
}

void Position::update_en_passant_in_make_(Move move)
{
    if(move.is_double_pawn_push())
        en_passant_square_ = square_in_between(move.from(), move.to());
    else
        en_passant_square_ = N_SQUARES;
}

void Position::update_halfclock_in_make_(Move move)
{
    if(move.piece_type() == PAWN || move.is_capture())
        halfclock_ = 0;
    else
        ++halfclock_;
}

void Position::update_moveclock_in_make_(Move move)
{
    if(side_to_move_ == BLACK)
        ++moveclock_;
}

void Position::unmake_quite_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.to(), move.from());
}

void Position::unmake_capture_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.to(), move.from());
    add_piece_(make_piece(toggle_color(side_to_move_), move.capture_piece_type()), move.to());
}

void Position::unmake_promotion_move_(Move move)
{
    remove_piece_(make_piece(side_to_move_, move.promotion_piece_type()), move.to());
    add_piece_(make_piece(side_to_move_, move.piece_type()), move.from());
}

void Position::unmake_capture_promotion_move_(Move move)
{
    remove_piece_(make_piece(side_to_move_, move.promotion_piece_type()), move.to());
    add_piece_(make_piece(side_to_move_, move.piece_type()), move.from());
    add_piece_(make_piece(toggle_color(side_to_move_), move.capture_piece_type()), move.to());
}

void Position::unmake_double_pawn_push_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.to(), move.from());
}

void Position::unmake_king_side_castling_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.to(), move.from());

    if(side_to_move_ == WHITE)
        move_piece_(make_piece(side_to_move_, ROOK), SQ_F1, SQ_H1);
    else
        move_piece_(make_piece(side_to_move_, ROOK), SQ_F8, SQ_H8);
}

void Position::unmake_queen_side_castling_move_(Move move)
{
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.to(), move.from());

    if(side_to_move_ == WHITE)
        move_piece_(make_piece(side_to_move_, ROOK), SQ_D1, SQ_A1);
    else
        move_piece_(make_piece(side_to_move_, ROOK), SQ_D8, SQ_A8);
}

void Position::unmake_en_passant_move_(Move move)
{
    Square capture_square = en_passant_capture_square(side_to_move_, move.to());

    add_piece_(make_piece(toggle_color(side_to_move_), PAWN), capture_square);
    move_piece_(make_piece(side_to_move_, move.piece_type()), move.to(), move.from());
}


void Position::update_castling_rights_in_unmake_(CastlingRights castling_rights)
{
    castling_rights_ = castling_rights;
}

void Position::update_en_passant_in_unmake_(Move move)
{
    if(move.is_en_passant())
        en_passant_square_ = move.to();
    else
        en_passant_square_ = N_SQUARES;
}

void Position::update_halfclock_in_unmake_(uint32_t halfclock)
{
    halfclock_ = halfclock;
}

void Position::update_moveclock_in_unmake_(Move move)
{
    if(side_to_move_ == BLACK)
        --moveclock_;
}



void Position::add_piece_(Piece piece, Square square)
{
    BitBoards::set_square(piece_bitboards_[piece], square);
}

void Position::remove_piece_(Piece piece, Square square)
{
    BitBoards::clear_square(piece_bitboards_[piece], square);
}

void Position::move_piece_(Piece piece, Square from, Square to)
{
    BitBoard from_to_BB = EMPTY_BB;
    /* Maybe add helper that returns bitboard with set square */
    BitBoards::set_square(from_to_BB, from);
    BitBoards::set_square(from_to_BB, to);

    piece_bitboards_[piece] ^= from_to_BB;
}