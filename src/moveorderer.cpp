#include "moveorderer.hpp"

constexpr uint32_t MVA_LVV_scores[N_PIECE_TYPES][N_PIECE_TYPES];
constexpr uint32_t promotion_scores[N_PIECE_TYPES];

MoveOrderer::MoveOrderer(MoveList *moves)
    : moves_(*moves), scores_(moves->size(), 0)
{
    assign_scores_();
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
    return moves_[current_index_++];
}

bool MoveOrderer::has_next() const
{
    return current_index_ < moves_.size();
}

void MoveOrderer::assign_scores_()
{
    for (int i = 0; i < moves_.size(); ++i)
    {
        auto move = moves_[i];
        auto &score = scores_[i];

        /*  Potential optimization: branches can be removed if MVA_LVV and promotion board 
            contain extra null score for non captures and non-promotions, but that requires 
            Move class to return N_PIECE_TYPE when calling capture_piece_type() and move is
            not capture*/
        if (move.is_capture())
            score += MVA_LVV_scores[move.piece_type()][move.capture_piece_type()];
        if (move.is_promotion())
            score += promotion_scores[move.promotion_piece_type()];
    }
}