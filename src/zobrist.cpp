#include "zobrist.hpp"

#include "defs.hpp"
#include "position.hpp"

#include <iostream>

constexpr size_t CASTLING_SIZE = 16;

uint64_t ZobristPieceKeys[N_PIECES][N_SQUARES];
uint64_t ZobristCastlingKeys[CASTLING_SIZE];
uint64_t ZobristEnPassantKeys[N_FILES];
uint64_t ZobristSideToMoveKey;

uint64_t xorshift64() {
    static uint64_t seed = 2366053040212936038ull;
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return seed;
}

void Zobrist::init() {
    for (Piece piece = WH_PAWN; piece < N_PIECES; ++piece)
        for (Square square = SQ_A1; square < N_SQUARES; ++square)
            ZobristPieceKeys[piece][square] = xorshift64();

    for (uint32_t i = 0; i < CASTLING_SIZE; ++i)
        ZobristCastlingKeys[i] = xorshift64();

    for (File file = FILE_A; file < N_FILES; ++file)
        ZobristEnPassantKeys[file] = xorshift64();

    ZobristSideToMoveKey = xorshift64();
}

void Zobrist::create_hashkey(Position &position) {
    for (Square square = SQ_A1; square < N_SQUARES; ++square)
        key_ ^= ZobristPieceKeys[position.piece_occupying(square)][square];

    key_ ^= ZobristCastlingKeys[position.castling_rights().raw()];

    if (position.en_passant_square() != N_SQUARES)
        key_ ^= ZobristEnPassantKeys[get_file(position.en_passant_square())];

    if (position.side_to_move() == BLACK)
        key_ ^= ZobristSideToMoveKey;

    last_castling_rights_ = position.castling_rights().raw();
    if (position.en_passant_square() != N_SQUARES)
        last_ep_ = get_file(position.en_passant_square());
}

Zobrist::HashKey Zobrist::hashkey() const { return key_; }

void Zobrist::add_piece(Piece piece, Square square) {
    key_ ^= ZobristPieceKeys[piece][square];
}

void Zobrist::remove_piece(Piece piece, Square square) {
    key_ ^= ZobristPieceKeys[piece][square];
}

void Zobrist::move_piece(Piece piece, Square from, Square to) {
    key_ ^= ZobristPieceKeys[piece][from];
    key_ ^= ZobristPieceKeys[piece][to];
}

void Zobrist::update_castling_rights(uint32_t castling_rights) {
    key_ ^= ZobristCastlingKeys[last_castling_rights_];
    key_ ^= ZobristCastlingKeys[castling_rights];
    last_castling_rights_ = castling_rights;
}

void Zobrist::update_en_passant(Square ep_square) {
    /* We can also make ZobristEnPassantKeys[N_SQUARES] 0 and remove branches */
    if (last_ep_ != N_SQUARES)
        key_ ^= ZobristEnPassantKeys[last_ep_];
    if (ep_square != N_SQUARES)
        key_ ^= ZobristEnPassantKeys[ep_square];
    last_ep_ = ep_square;
}

void Zobrist::update_side_to_move() {
    key_ ^= ZobristSideToMoveKey;
}