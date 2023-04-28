#ifndef ALTAIR_POSITION_HPP
#define ALTAIR_POSITION_HPP

#include <string>
#include <array>

#include "defs.hpp"
#include "move.hpp"

class Position
{
    public:
    struct IrrecoverableState
    {
        CastlingRights castling_rights;
        uint32_t halfclock;
    };

    public:
    Position();
    explicit Position(const std::string& fen);

    void set_to_starting();
    void set_from_fen(const std::string& fen);
    std::string fen() const;
    std::string pretty() const;

    void make_move(Move move);
    void unmake_move(Move move, IrrecoverableState irrecoverable_state);

    BitBoard piece_bitboard(Color color, PieceType type) const;
    BitBoard occupancy_bitboard() const;
    BitBoard occupancy_bitboard(Color color) const;
    
    Piece piece_occupying(Square square) const;

    bool is_in_check(Color color) const;
    bool is_square_attacked(Square square, Color color) const;
    BitBoard square_attackers(Square square) const;

    Color side_to_move() const;
    CastlingRights castling_rights() const;
    Square en_passant_square() const;
    uint32_t halfclock() const;
    uint32_t moveclock() const;

    IrrecoverableState irrecoverable_state() const;

    public:
    /* Maybe not the best place for this function */
    static Square en_passant_capture_square(Color side_to_move, Square en_passant_square);

    private:

    void clear_piece_bitboards_();

    void make_quite_move_                   (Move move);
    void make_capture_move_                 (Move move);
    void make_promotion_move_               (Move move);
    void make_capture_promotion_move_       (Move move);
    void make_double_pawn_push_move_        (Move move);
    void make_king_side_castling_move_      (Move move);
    void make_queen_side_castling_move_     (Move move);
    void make_en_passant_move_              (Move move);

    void update_castling_rights_in_make_    (Move move);
    void update_en_passant_in_make_         (Move move);
    void update_halfclock_in_make_          (Move move);
    void update_moveclock_in_make_          (Move move);

    void unmake_quite_move_                 (Move move);
    void unmake_capture_move_               (Move move);
    void unmake_promotion_move_             (Move move);
    void unmake_capture_promotion_move_     (Move move);
    void unmake_double_pawn_push_move_      (Move move);
    void unmake_king_side_castling_move_    (Move move);
    void unmake_queen_side_castling_move_   (Move move);
    void unmake_en_passant_move_            (Move move);

    void update_castling_rights_in_unmake_  (CastlingRights castling_rights);
    void update_en_passant_in_unmake_       (Move move);
    void update_halfclock_in_unmake_        (uint32_t halfclock);
    void update_moveclock_in_unmake_        (Move move);

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