#ifndef GWAIHIR_ZOBRIST_HPP
#define GWAIHIR_ZOBRIST_HPP

#include "defs.hpp"

#include <cstdint>

class Position;

class Zobrist 
{
    public:
    using HashKey = uint64_t; 
    
    static void init();

    HashKey hashkey() const;

    void create_hashkey(Position& position);

    void add_piece(Piece piece, Square square);
    void remove_piece(Piece piece, Square square);
    void move_piece(Piece piece, Square from, Square to);

    void update_castling_rights(uint32_t castling_rights);
    void update_en_passant(Square ep_square);
    void update_side_to_move();

    private:
    HashKey key_;

    uint32_t last_castling_rights_;
    Square last_ep_ = N_SQUARES;
};

#endif // GWAIHIR_ZOBRIST_HPP