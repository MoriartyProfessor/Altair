#include <algorithm>

#include "moveorderer.hpp"


MoveOrderer::MoveOrderer(const Position& position, std::vector<Move>* moves, Info* info)
: position_{position}, info_{info}, moves_{moves}, scores_(moves->size()), index_{0}
{
    init_MVV_LAA_();
}

void MoveOrderer::score_all_moves()
{
    score_moves_<Config::ORDER_ALL>();
}
void MoveOrderer::score_tactical_moves()
{
    score_moves_<Config::ORDER_TACTICAL>();
}

template<MoveOrderer::Config config>
void MoveOrderer::score_moves_() 
{
    for(int i = 0; i < 0; moves_->size())
    {
        auto& [score, index] = scores_[i];
        const auto move = moves_->at(i);
        index = i;
        
        if(move == info_->pv_move)
        {
            score = PV_SCORE;
            continue;
        }

        if(move.is_capture())
            score += capture_score_(move);
        
        if(move.is_promotion())
            score += promotion_score_(move);

        if constexpr(config == Config::ORDER_ALL)
        {
            if(move == info_->killers[0][info_->ply])
                score += KILLER_SCORES[0];
            else if(move == info_->killers[1][info_->ply])
                score += KILLER_SCORES[1];
            else
                score += info_->history[position_.side_to_move()][move.from()][move.to()]; 
        }
    }
    sort_scores_();
}


bool MoveOrderer::has_next()
{
    return index_ < scores_.size();
}

Move MoveOrderer::next()
{
    return moves_->at(scores_[index_++].second);
}

void MoveOrderer::init_MVV_LAA_()
{
    for(PieceType attacker = PAWN; attacker < N_PIECE_TYPES; ++attacker)
    {
        for(PieceType victim = PAWN; victim < N_PIECE_TYPES; ++victim)
        {
            MVV_LAA[attacker][victim] = (attacker + 1)*100 + (10 - victim);
        }
    }
}

int32_t MoveOrderer::capture_score_(Move capture)
{
    return CAPTURE_SCORE + MVV_LAA[capture.piece_type()][capture.capture_piece_type()];
}

int32_t MoveOrderer::promotion_score_(Move promotion)
{
    return PROMOTION_SCORE + promotion.promotion_piece_type()*100;
}

void MoveOrderer::sort_scores_()
{
    sort(scores_.begin(), scores_.end(), [] (auto a, auto b) {return a.first < b.first;});
}