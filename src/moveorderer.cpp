#include "moveorderer.hpp"

constexpr auto generate_mvv_lva() 
{
    std::array<std::array<uint32_t, N_PIECE_TYPES>, N_PIECE_TYPES> mvv_lva_scores = {};

    constexpr uint32_t piece_scores[N_PIECE_TYPES] = {10, 30, 35, 50, 90, 1000};

    for (PieceType attacker = PAWN; attacker != N_PIECE_TYPES; ++attacker) 
    {
        for (PieceType victim = PAWN; victim != N_PIECE_TYPES; ++victim) 
        {
            mvv_lva_scores[attacker][victim] = piece_scores[victim] * 10 - piece_scores[attacker];
        }    
    }
    return mvv_lva_scores;
}

constexpr uint32_t HASH_MOVE_SCORE = 5000;
constexpr uint32_t KILLER_MOVE_SCORE = 90;
constexpr auto MVV_LVA_SCORES = generate_mvv_lva();
constexpr uint32_t PROMOTION_SCORES[N_PIECE_TYPES] = {0, 1000, 1100, 1500, 2000, 0};

MoveOrderer::MoveOrderer(MoveList &moves, std::pair<Move, Move> killers, Move hash_move)
    : moves_(moves), scores_(moves.size(), 0)
{
    assign_scores_(killers, hash_move);
}

Move MoveOrderer::next()
{
    uint32_t i, max_score = 0, max_index = current_index_;
    for (i = current_index_; i < moves_.size(); ++i)
    {
        if (scores_[i] > max_score)
        {
            max_score = scores_[i];
            max_index = i;
        }
    }
    std::swap(moves_[max_index], moves_[current_index_]);
    std::swap(scores_[max_index], scores_[current_index_]);
    return moves_[current_index_++];
}

bool MoveOrderer::has_next() const
{
    return current_index_ < moves_.size();
}

void MoveOrderer::assign_scores_(std::pair<Move, Move> killers, Move hash_move)
{
    for (int i = 0; i < moves_.size(); ++i)
    {
        auto move = moves_[i];
        auto &score = scores_[i];

        /*  Potential optimization: branches can be removed if MVV_LVA and promotion board 
            contain extra null score for non captures and non-promotions, but that requires 
            Move class to return N_PIECE_TYPE when calling capture_piece_type() and move is
            not capture*/
        if (move == hash_move)
            score += HASH_MOVE_SCORE;
        if (move == killers.first || move == killers.second)
            score += KILLER_MOVE_SCORE;
        if (move.is_capture())
            score += MVV_LVA_SCORES[move.piece_type()][move.capture_piece_type()];
        if (move.is_promotion())
            score += PROMOTION_SCORES[move.promotion_piece_type()];
    }
}