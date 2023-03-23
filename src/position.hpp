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
    Position(const std::string& fen);

    void set_to_starting();
    void set_to_fen(std::string& fen);
    std::string fen() const;

    void do_move(Move move);

    BitBoard piece_bitboard(Color color, PieceType type) const;
    BitBoard all_pieces_bitboard() const;
    
    Piece piece_occupying(Square square) const;
    
    Color side_to_play() const;
    CastlingRights castling_rights() const;
    Square en_passant_square() const;
    uint32_t halfclock() const;

    private:
    std::array<BitBoard, N_PIECES> piece_bitboards_;
    CastlingRights castling_rights_;
    Color side_to_play_ = WHITE;
    uint32_t halfclock_ = 0;
};

#endif // ALTAIR_POSITION_HPP