#ifndef ALTAIR_MOVE_HPP
#define ALTAIR_MOVE_HPP

#include <string>

#include "defs.hpp"

class Move
{
    public:

    Move() = default;
    Move(uint32_t move);

    Move& operator= (Move other);
    // either don't take into account unused bits, or make sure they are always set to zero
    bool operator== (Move other) const;
    bool operator!= (Move other) const;

    Square from() const;
    Square to() const;
    PieceType piece_type() const;

    PieceType capture_piece_type(PieceType type) const;
    PieceType promotion_piece_type(PieceType type) const;

    bool is_quite() const;
    bool is_capture() const;
    bool is_promotion() const;
    bool is_double_pawn_push() const;
    bool is_king_side_castle() const;
    bool is_queen_side_castle() const;
    bool is_en_passant() const;

    void set_from(Square from);
    void set_to(Square to);
    void set_piece_type(PieceType type);

    void set_capture_piece_type(PieceType type);
    void set_promotion_piece_type(PieceType type);

    void set_quite();
    void set_capture();
    void set_promotion();
    void set_double_pawn_push();
    void set_king_side_castle();
    void set_queen_side_castle();
    void set_en_passant();

    std::string descriptive_notation() const;
    std::string algebraic_notation() const;

    private:

    enum FlagBit
    {
        QUITE,
        CAPTURE,
        PROMOTION,
        DOUBLE_PAWN_PUSH,
        KING_CASTLE,
        QUEEN_CASTLE,
        EN_PASSANT
    };

    void set_flag(FlagBit flag);

    uint32_t move_ = 0;
};


#endif // ALTAIR_MOVE_HPP