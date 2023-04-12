#include "movegenerator.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "patterns.hpp"

MoveGenerator::MoveGenerator(const Position& position)
: position_{position}
{
    gen_pseudolegal_moves_();
}

void MoveGenerator::set_position(const Position& position)
{
    position_ = position;
    gen_pseudolegal_moves_();
}


std::vector<Move> MoveGenerator::legal_moves()
{
    /* Generate legal moves only on-demand */
    gen_legal_moves_();
    return legal_moves_;
}

std::vector<Move> MoveGenerator::pseudolegal_moves()
{
    return pseudolegal_moves_;
}



void MoveGenerator::gen_legal_moves_()
{

}

void MoveGenerator::gen_pseudolegal_moves_()
{
    gen_knight_moves_();
    gen_king_moves_();
}


void MoveGenerator::gen_knight_moves_()
{
    BitBoard knight_bitboard = position_.piece_bitboard(position_.side_to_move(), KNIGHT);

    for(Square knight = pop_LSB(knight_bitboard); knight != N_SQUARES; knight = pop_LSB(knight_bitboard))
    {
        BitBoard knight_attacks = Patterns::get_knight_attacks(knight) & ~position_.occupancy_bitboard(position_.side_to_move());
        add_piece_moves_(KNIGHT, knight, knight_attacks);
    }
}

void MoveGenerator::gen_king_moves_()
{
    /* Maybe generalize with other gen functions */
    BitBoard king_bitboard = position_.piece_bitboard(position_.side_to_move(), KING);

    for(Square king = pop_LSB(king_bitboard); king != N_SQUARES; king = pop_LSB(king_bitboard))
    {
        BitBoard king_attacks = Patterns::get_king_attacks(king) & ~position_.occupancy_bitboard(position_.side_to_move());
        add_piece_moves_(KING, king, king_attacks);
    }
}


void MoveGenerator::add_piece_moves_(PieceType type, Square from, BitBoard attacks)
{
    for(Square attack = pop_LSB(attacks); attack != N_SQUARES; attack = pop_LSB(attacks))
    {
        Move move{from, attack};
        move.set_piece_type(type);

        Piece attacked_piece = position_.piece_occupying(attack);
        if(attacked_piece != N_PIECES)
        {
            move.set_capture();
            move.set_capture_piece_type(get_type(attacked_piece));
        }
        else
        {
            move.set_quite();
        }

        pseudolegal_moves_.push_back(move);
    }
}