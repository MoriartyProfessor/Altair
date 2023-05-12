#ifndef ALTAIR_DEFS_HPP
#define ALTAIR_DEFS_HPP

#include <cstdint>
#include <string>

// Might want to use more compact integers in future
using BitBoard = uint64_t;
using PieceType = uint32_t;
using Piece = uint32_t;
using Color = uint32_t;
using Square = uint32_t;
using File = uint32_t;
using Rank = uint32_t;
using Direction = uint32_t;

const uint32_t MAX_DEPTH = 64;
const uint32_t MAX_PLY = 256;
const uint32_t MOVE_LIST_SIZE = 128;

enum PieceTypes : PieceType 
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    
    N_PIECE_TYPES
};

enum Pieces : Piece 
{
    WH_PAWN,    BL_PAWN,
    WH_KNIGHT,  BL_KNIGHT,
    WH_BISHOP,  BL_BISHOP,
    WH_ROOK,    BL_ROOK,
    WH_QUEEN,   BL_QUEEN,
    WH_KING,    BL_KING,
    N_PIECES
};

enum Colors : Color 
{
    WHITE,
    BLACK,

    N_COLORS
};

enum Squares : Square
{
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,

    N_SQUARES
};

enum Files : File
{
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,

    N_FILES
};

enum Ranks : Rank
{
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,

    N_RANKS
};

enum Directions : Direction
{
    NORTH,
    NORTH_WEST,
    WEST,
    SOUTH_WEST,
    SOUTH,
    SOUTH_EAST,
    EAST,
    NORTH_EAST,

    N_DIRECTIONS
};

struct CastlingRights
{
    void set_queen_side(Color color) {rights_ |= (1 << color);}
    void clear_queen_side(Color color) {rights_ &= ~(1 << color);}
    void set_king_side(Color color) {rights_ |= (1 << (color + 2));}
    void clear_king_side(Color color) {rights_ &= ~(1 << (color + 2));}

    void set_all_color_rights(Color color) {rights_ |= (0x5 << color);}
    void clear_all_color_rights(Color color) {rights_ &= ~(0x5 << color);}

    void set_all_rights() {rights_ = 0xF;}
    void clear_all_rights() {rights_ = 0x0;}

    bool queen_side(Color color) const {return rights_ & (1 << color);}
    bool king_side(Color color) const {return rights_ & (1 << (color + 2));}

    bool is_all_set() const {return rights_ == 0xF;}
    bool is_all_clear() const {return rights_ == 0x0;}
    
    private:
    /* 32-bit for now, it is more cache friendly */
    uint32_t rights_ = 0xF;
};


template<Direction direction> inline Square step(Square square) { static_assert(direction < N_DIRECTIONS); return N_SQUARES;} // returning to shut warnings}
template<> inline Square step<NORTH>        (Square square) { return square + 8; }
template<> inline Square step<NORTH_WEST>   (Square square) { return square + 7; }
template<> inline Square step<WEST>         (Square square) { return square - 1; }
template<> inline Square step<SOUTH_WEST>   (Square square) { return square - 9; }
template<> inline Square step<SOUTH>        (Square square) { return square - 8; }
template<> inline Square step<SOUTH_EAST>   (Square square) { return square - 7; }
template<> inline Square step<EAST>         (Square square) { return square + 1; }
template<> inline Square step<NORTH_EAST>   (Square square) { return square + 9; }

template<Direction direction> inline Square step(Square square, uint32_t n) { static_assert(direction < N_DIRECTIONS); return N_SQUARES;}
template<> inline Square step<NORTH>        (Square square, uint32_t n) { return square + 8 * n; }
template<> inline Square step<NORTH_WEST>   (Square square, uint32_t n) { return square + 7 * n; }
template<> inline Square step<WEST>         (Square square, uint32_t n) { return square - 1 * n; }
template<> inline Square step<SOUTH_WEST>   (Square square, uint32_t n) { return square - 9 * n; }
template<> inline Square step<SOUTH>        (Square square, uint32_t n) { return square - 8 * n; }
template<> inline Square step<SOUTH_EAST>   (Square square, uint32_t n) { return square - 7 * n; }
template<> inline Square step<EAST>         (Square square, uint32_t n) { return square + 1 * n; }
template<> inline Square step<NORTH_EAST>   (Square square, uint32_t n) { return square + 9 * n; }


inline File get_file(Square square)
{
    return square & 0x7;
}

inline Rank get_rank(Square square)
{
    return square >> 3;
}

inline Square make_square(File file, Rank rank)
{
    return (rank << 3) + file;
}

inline Square make_piece(Color color, PieceType type)
{
    return (type << 1) + color;
}

inline Color toggle_color(Color color)
{
    return color ^ BLACK;
}

inline Color get_color(Piece piece)
{
    return piece & 0x1;
}

inline PieceType get_type(Piece piece)
{
    return piece >> 1;
}

constexpr bool is_direction_positive(Direction direction)
{
    return direction == EAST || direction == NORTH || direction == NORTH_EAST || direction == NORTH_WEST;
}

inline Square square_in_between(Square square_1, Square square_2)
{
    return square_1 + ((static_cast<int32_t>(square_2) - static_cast<int32_t>(square_1))/2);
}

inline Direction invert_direction(Direction direction)
{
    return (direction + 4) % N_DIRECTIONS;
}

inline char piece_to_char(Piece piece)
{
    if(piece == N_PIECES)
        return '.';
    
    PieceType type = get_type(piece);
    Color color = get_color(piece);

    char piece_ch;

    if(type == PAWN)
        piece_ch = 'P';
    if(type == KNIGHT)
        piece_ch = 'N';
    if(type == BISHOP)
        piece_ch = 'B';
    if(type == ROOK)
        piece_ch = 'R';
    if(type == QUEEN)
        piece_ch = 'Q';
    if(type == KING)
        piece_ch = 'K';

    if(color == BLACK)
        piece_ch += 32;
    return piece_ch;
}

inline Piece char_to_piece(char piece_char)
{
    Color color = piece_char >= 'A' && piece_char <= 'Z' ? WHITE : BLACK;

    if(color == BLACK)
        piece_char -= 32;

    PieceType type;
    if(piece_char == 'P')
        type = PAWN;
    if(piece_char == 'N')
        type = KNIGHT;
    if(piece_char == 'B')
        type = BISHOP;
    if(piece_char == 'R')
        type = ROOK;
    if(piece_char == 'Q')
        type = QUEEN;
    if(piece_char == 'K')
        type = KING;

    return make_piece(color, type);
}

inline char file_to_char(File file)
{
    return file + 'a';
}

inline char rank_to_char(Rank rank)
{
    return rank + '1';
}

inline std::string square_to_str(Square square)
{
    return std::string{file_to_char(get_file(square)), rank_to_char(get_rank(square))};
}

inline File char_to_file(char file_char)
{
    return file_char - 'a';
}

inline Rank char_to_rank(char rank_char)
{
    return rank_char - '1';
}

inline Square str_to_square(const std::string& square_str)
{
    return make_square(char_to_file(square_str[0]), char_to_rank(square_str[1]));
}

#endif // ALTAIR_DEFS_HPP