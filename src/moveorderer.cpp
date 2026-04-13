#include "moveorderer.hpp"
#include <cstddef>


constexpr auto generate_mvv_lva() 
{
    std::array<std::array<int32_t, N_PIECE_TYPES>, N_PIECE_TYPES> mvv_lva_scores = {};

    constexpr int32_t piece_scores[N_PIECE_TYPES] = {10, 30, 35, 50, 90, 1000};

    for (PieceType attacker = PAWN; attacker != N_PIECE_TYPES; ++attacker) 
    {
        for (PieceType victim = PAWN; victim != N_PIECE_TYPES; ++victim) 
        {
            mvv_lva_scores[attacker][victim] = piece_scores[victim] * 10 - piece_scores[attacker] + 100000;
        }
    }
    return mvv_lva_scores;
}

constexpr int32_t HASH_MOVE_SCORE = 500000;
constexpr int32_t KILLER_MOVE_SCORES[] = {90000, 80000};

constexpr auto MVV_LVA_SCORES = generate_mvv_lva();
constexpr int32_t PROMOTION_SCORES[N_PIECE_TYPES] = {0, 10000, 11000, 150000, 200000, 0};

MoveOrderer::MoveOrderer(MoveList &moves, std::pair<Move, Move> killers,
                         int32_t history_scores[N_SQUARES][N_SQUARES],
                         Move hash_move)
    : moves_(moves), scores_(moves.size(), 0)
{
    assign_scores_(killers, history_scores, hash_move);
}

Move MoveOrderer::next()
{
    int32_t i, max_score = 0, max_index = current_index_;
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

void MoveOrderer::assign_scores_(std::pair<Move, Move> killers,
                                 int32_t history_scores[N_SQUARES][N_SQUARES],
                                 Move hash_move)
{
    for (size_t i = 0; i < moves_.size(); ++i)
    {
        auto move = moves_[i];
        auto &score = scores_[i];

        if (move == hash_move)
            score += HASH_MOVE_SCORE;
        if (move.is_quiet())
        {
            if (move == killers.first)
                score += KILLER_MOVE_SCORES[0];
            else if (move == killers.second)
                score += KILLER_MOVE_SCORES[1];
            else if (history_scores != nullptr)
                score += history_scores[move.from()][move.to()];
        }
        if (move.is_capture())
            score += MVV_LVA_SCORES[move.piece_type()][move.capture_piece_type()];
        if (move.is_promotion())
            score += PROMOTION_SCORES[move.promotion_piece_type()];
    }
}