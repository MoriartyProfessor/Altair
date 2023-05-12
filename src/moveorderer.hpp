#ifndef ALTAIR_MOVEORDERER_HPP
#define ALTAIR_MOVEORDERER_HPP

#include <vector>

#include "defs.hpp"
#include "move.hpp"
#include "position.hpp"

class MoveOrderer
{
    public:
    struct Info
    {
        Move pv_move;
        uint32_t history [N_COLORS][N_SQUARES][N_SQUARES];
        Move killers [2][MAX_PLY];
        uint32_t ply;
    };

    enum class Config
    {
        /* Semantically confusing */
        ORDER_ALL,
        ORDER_TACTICAL
    };

    explicit MoveOrderer(const Position& position, std::vector<Move>* moves, Info* info);

    template<Config config>
    void score_moves();

    bool has_next();

    Move next();

    private:
    void init_MVV_LAA_();
    int32_t capture_score_(Move capture);
    int32_t promotion_score_(Move promotion);
    void sort_scores_();

    private:
    const Position& position_;
    const Info* info_;
    std::vector<Move>* moves_;
    std::vector<std::pair<int32_t, uint32_t>> scores_;
    uint32_t index_;

    static inline int32_t MVV_LAA[N_PIECE_TYPES][N_PIECE_TYPES];
    const int32_t PV_SCORE = 30000;
    const int32_t KILLER_SCORES[2] = {50, 45};
    const int32_t CAPTURE_SCORE = 20;
    const int32_t PROMOTION_SCORE = 50;
};

#endif // ALTAIR_MOVEORDERER_HPP