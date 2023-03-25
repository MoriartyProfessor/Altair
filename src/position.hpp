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

    void do_move(Move move);

    BitBoard piece_bitboard(Color color, PieceType type) const;
    BitBoard occupancy_bitboard() const;
    
    Piece piece_occupying(Square square) const;
    
    Color side_to_move() const;
    CastlingRights castling_rights() const;
    Square en_passant_square() const;
    uint32_t halfclock() const;
    uint32_t moveclock() const;

    private:

    void clear_piece_bitboards();

    private:
    std::array<BitBoard, N_PIECES> piece_bitboards_;
    Color side_to_move_ = WHITE;
    CastlingRights castling_rights_;
    Square en_passant_square_ = N_SQUARES;
    uint32_t halfclock_ = 0;
    uint32_t moveclock_ = 0;
};

#endif // ALTAIR_POSITION_HPP