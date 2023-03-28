#ifndef ALTAIR_POSITION_HPP
#define ALTAIR_POSITION_HPP

#include <string>
#include <array>

#include "defs.hpp"
#include "move.hpp"

class Position
{
    public:
    Position();
    explicit Position(const std::string& fen);

    void set_to_starting();
    void set_from_fen(const std::string& fen);
    std::string fen() const;
    std::string pretty() const;

    void make_move(Move move);
    void unmake_move(Move move);

    BitBoard piece_bitboard(Color color, PieceType type) const;
    BitBoard occupancy_bitboard() const;
    BitBoard occupancy_bitboard(Color color) const;
    
    Piece piece_occupying(Square square) const;

    bool is_in_check(Color color) const;
    
    Color side_to_move() const;
    CastlingRights castling_rights() const;
    Square en_passant_square() const;
    uint32_t halfclock() const;
    uint32_t moveclock() const;

    private:

    void clear_piece_bitboards_();

    void make_quite_move_               (Move move);
    void make_capture_move_             (Move move);
    void make_promotion_move_           (Move move);
    void make_capture_promotion_move_   (Move move);
    void make_double_pawn_push_move_    (Move move);
    void make_king_side_castling_move_  (Move move);
    void make_queen_side_castling_move_ (Move move);
    void make_en_passant_move_          (Move move);

    void add_piece_(Piece piece, Square square);
    /* Maybe add toggle piece interface instead*/
    void remove_piece_(Piece piece, Square square);
    void move_piece_(Piece piece, Square from, Square to);

    private:
    std::array<BitBoard, N_PIECES> piece_bitboards_;
    Color side_to_move_ = WHITE;
    CastlingRights castling_rights_;
    Square en_passant_square_ = N_SQUARES;
    uint32_t halfclock_ = 0;
    uint32_t moveclock_ = 0;
};

#endif // ALTAIR_POSITION_HPP