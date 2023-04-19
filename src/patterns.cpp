#include "patterns.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "rays.hpp"

namespace Patterns::Magic
{
    BitBoard attack_table_[107648];

    struct Info
    {
        BitBoard* attack_table;
        BitBoard occupancy_mask;
        uint64_t magic;
        uint32_t bit_index;
    };

    Info bishop_info_table_[N_SQUARES];
    Info rook_info_table_[N_SQUARES];

    BitBoard gen_bishop_occupancy_mask_(Square square)
    {
        constexpr BitBoard rim_BB = FILE_A_BB | FILE_H_BB | RANK_1_BB | RANK_8_BB;

        BitBoard bishop_occupancy_mask =   (Rays::get(NORTH_WEST, square) | 
                                            Rays::get(SOUTH_WEST, square) | 
                                            Rays::get(SOUTH_EAST, square) | 
                                            Rays::get(NORTH_EAST, square)) &
                                            ~rim_BB;
        return bishop_occupancy_mask;
    }

    BitBoard gen_rook_occupancy_mask_(Square square)
    {
        BitBoard rook_occupancy_mask =   (Rays::get(NORTH, square) & ~RANK_8_BB) |
                                         (Rays::get(WEST,  square) & ~FILE_A_BB) |
                                         (Rays::get(SOUTH, square) & ~RANK_1_BB) |
                                         (Rays::get(EAST,  square) & ~FILE_H_BB) ;
        return rook_occupancy_mask;
    }

    BitBoard gen_occupancy_candidate_(BitBoard occupancy_mask, uint32_t index)
    {
        BitBoard occupancy_candidate = EMPTY_BB;
        uint32_t bit_index = popcount(occupancy_mask);
        for(uint32_t i = 0; i < bit_index; ++i, index >>= 1)
        {
            Square lsb = pop_LSB(occupancy_mask);
            if(index & 1)
                BitBoards::set_square(occupancy_candidate, lsb);
        }
        return occupancy_candidate;
    }

    void init_info_tables_()
    {
        /* Magic numbers are taken from Koivisto for now */
        constexpr uint64_t bishop_magics[N_SQUARES] = {
                            0x0002020202020200ULL, 0x0002020202020000ULL, 0x0004010202000000ULL, 0x0004040080000000ULL,
                            0x0001104000000000ULL, 0x0000821040000000ULL, 0x0000410410400000ULL, 0x0000104104104000ULL,
                            0x0000040404040400ULL, 0x0000020202020200ULL, 0x0000040102020000ULL, 0x0000040400800000ULL,
                            0x0000011040000000ULL, 0x0000008210400000ULL, 0x0000004104104000ULL, 0x0000002082082000ULL,
                            0x0004000808080800ULL, 0x0002000404040400ULL, 0x0001000202020200ULL, 0x0000800802004000ULL,
                            0x0000800400A00000ULL, 0x0000200100884000ULL, 0x0000400082082000ULL, 0x0000200041041000ULL,
                            0x0002080010101000ULL, 0x0001040008080800ULL, 0x0000208004010400ULL, 0x0000404004010200ULL,
                            0x0000840000802000ULL, 0x0000404002011000ULL, 0x0000808001041000ULL, 0x0000404000820800ULL,
                            0x0001041000202000ULL, 0x0000820800101000ULL, 0x0000104400080800ULL, 0x0000020080080080ULL,
                            0x0000404040040100ULL, 0x0000808100020100ULL, 0x0001010100020800ULL, 0x0000808080010400ULL,
                            0x0000820820004000ULL, 0x0000410410002000ULL, 0x0000082088001000ULL, 0x0000002011000800ULL,
                            0x0000080100400400ULL, 0x0001010101000200ULL, 0x0002020202000400ULL, 0x0001010101000200ULL,
                            0x0000410410400000ULL, 0x0000208208200000ULL, 0x0000002084100000ULL, 0x0000000020880000ULL,
                            0x0000001002020000ULL, 0x0000040408020000ULL, 0x0004040404040000ULL, 0x0002020202020000ULL,
                            0x0000104104104000ULL, 0x0000002082082000ULL, 0x0000000020841000ULL, 0x0000000000208800ULL,
                            0x0000000010020200ULL, 0x0000000404080200ULL, 0x0000040404040400ULL, 0x0002020202020200ULL
                        };
        constexpr uint64_t rook_magics[N_SQUARES] = {
                            0x0080001020400080ULL, 0x0040001000200040ULL, 0x0080081000200080ULL, 0x0080040800100080ULL,
                            0x0080020400080080ULL, 0x0080010200040080ULL, 0x0080008001000200ULL, 0x0080002040800100ULL,
                            0x0000800020400080ULL, 0x0000400020005000ULL, 0x0000801000200080ULL, 0x0000800800100080ULL,
                            0x0000800400080080ULL, 0x0000800200040080ULL, 0x0000800100020080ULL, 0x0000800040800100ULL,
                            0x0000208000400080ULL, 0x0000404000201000ULL, 0x0000808010002000ULL, 0x0000808008001000ULL,
                            0x0000808004000800ULL, 0x0000808002000400ULL, 0x0000010100020004ULL, 0x0000020000408104ULL,
                            0x0000208080004000ULL, 0x0000200040005000ULL, 0x0000100080200080ULL, 0x0000080080100080ULL,
                            0x0000040080080080ULL, 0x0000020080040080ULL, 0x0000010080800200ULL, 0x0000800080004100ULL,
                            0x0000204000800080ULL, 0x0000200040401000ULL, 0x0000100080802000ULL, 0x0000080080801000ULL,
                            0x0000040080800800ULL, 0x0000020080800400ULL, 0x0000020001010004ULL, 0x0000800040800100ULL,
                            0x0000204000808000ULL, 0x0000200040008080ULL, 0x0000100020008080ULL, 0x0000080010008080ULL,
                            0x0000040008008080ULL, 0x0000020004008080ULL, 0x0000010002008080ULL, 0x0000004081020004ULL,
                            0x0000204000800080ULL, 0x0000200040008080ULL, 0x0000100020008080ULL, 0x0000080010008080ULL,
                            0x0000040008008080ULL, 0x0000020004008080ULL, 0x0000800100020080ULL, 0x0000800041000080ULL,
                            0x00FFFCDDFCED714AULL, 0x007FFCDDFCED714AULL, 0x003FFFCDFFD88096ULL, 0x0000040810002101ULL,
                            0x0001000204080011ULL, 0x0001000204000801ULL, 0x0001000082000401ULL, 0x0001FFFAABFAD1A2ULL
                        };
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            Magic::Info& bishop_info = bishop_info_table_[square];
            bishop_info.magic = bishop_magics[square];
            bishop_info.occupancy_mask = gen_bishop_occupancy_mask_(square);
            bishop_info.bit_index = popcount(bishop_info.occupancy_mask);

            Magic::Info& rook_info = rook_info_table_[square];
            rook_info.magic = rook_magics[square];
            rook_info.occupancy_mask = gen_rook_occupancy_mask_(square);
            rook_info.bit_index = popcount(rook_info.occupancy_mask);
        }

        BitBoard* attack_table_ptr = attack_table_;
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            bishop_info_table_[square].attack_table = attack_table_ptr;
            attack_table_ptr += (1 << bishop_info_table_[square].bit_index);
        }
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            rook_info_table_[square].attack_table = attack_table_ptr;
            attack_table_ptr += (1 << rook_info_table_[square].bit_index);
        }
    }

    template <Direction direction>
    BitBoard gen_ray_attacks(Square square, BitBoard occupancy_bitboard)
    {
        BitBoard ray_attacks = Rays::get(direction, square);
        BitBoard blockers = ray_attacks & occupancy_bitboard;
        if constexpr (is_direction_positive(direction))
        {
            if(blockers)
            {
                Square first_blocker = LSB(blockers);
                ray_attacks ^= Rays::get(direction, first_blocker);
            }
        }
        else
        {
            if(blockers)
            {
                Square first_blocker = MSB(blockers);
                ray_attacks ^= Rays::get(direction, first_blocker);
            }
        }
        return ray_attacks;
    }

    BitBoard gen_bishop_attacks_(Square square, BitBoard occupancy_bitboard)
    {
        /* Maybe use template to combine with gen_rook_attacks_ */
        BitBoard bishop_attacks = EMPTY_BB;

        bishop_attacks |= gen_ray_attacks<NORTH_WEST>(square, occupancy_bitboard);
        bishop_attacks |= gen_ray_attacks<SOUTH_WEST>(square, occupancy_bitboard);
        bishop_attacks |= gen_ray_attacks<SOUTH_EAST>(square, occupancy_bitboard);
        bishop_attacks |= gen_ray_attacks<NORTH_EAST>(square, occupancy_bitboard);

        return bishop_attacks;
    }

    BitBoard gen_rook_attacks_(Square square, BitBoard occupancy_bitboard)
    {
        BitBoard rook_attacks = EMPTY_BB;

        rook_attacks |= gen_ray_attacks<NORTH>(square, occupancy_bitboard);
        rook_attacks |= gen_ray_attacks<WEST>(square, occupancy_bitboard);
        rook_attacks |= gen_ray_attacks<SOUTH>(square, occupancy_bitboard);
        rook_attacks |= gen_ray_attacks<EAST>(square, occupancy_bitboard);

        return rook_attacks;
    }

    void init_attack_table_()
    {
        for(Square square = SQ_C4; square == SQ_C4; ++square)
        {
            /* Refactor, combine loops and simplify */
            Info& bishop_info = bishop_info_table_[square];
            uint32_t max_index = (1 << bishop_info.bit_index) - 1;
            for(uint32_t index = 0; index <= max_index; ++index)
            {
                BitBoard occupancy = gen_occupancy_candidate_(bishop_info.occupancy_mask, index);
                BitBoard attacks = gen_bishop_attacks_(square, occupancy);
                
                occupancy *= bishop_info.magic;
                occupancy >>= (N_SQUARES - bishop_info.bit_index);
                bishop_info.attack_table[occupancy] = attacks;
            }
            
            Info& rook_info = rook_info_table_[square];
            max_index = (1 << rook_info.bit_index) - 1;
            for(uint32_t index = 0; index <= max_index; ++index)
            {
                BitBoard occupancy = gen_occupancy_candidate_(rook_info.occupancy_mask, index);
                BitBoard attacks = gen_rook_attacks_(square, occupancy);
                occupancy *= rook_info.magic;
                occupancy >>= N_SQUARES - rook_info.bit_index;
                rook_info.attack_table[occupancy] = attacks;
            }
        }
    }

    void init_()
    {
        init_info_tables_();
        init_attack_table_();
    }
}

namespace Patterns
{
    BitBoard knight_attacks_[N_SQUARES];
    BitBoard king_attacks_[N_SQUARES];

    BitBoard gen_knight_attacks_(Square square)
    {
        BitBoard knight_bitboard = BitBoards::square_set_in(square);
        BitBoard knight_attacks = EMPTY_BB;

        knight_attacks |= (knight_bitboard << 17) & (~FILE_A_BB)            ;
        knight_attacks |= (knight_bitboard << 10) & (~(FILE_A_BB|FILE_B_BB));
        knight_attacks |= (knight_bitboard >>  6) & (~(FILE_A_BB|FILE_B_BB));
        knight_attacks |= (knight_bitboard >> 15) & (~FILE_A_BB)            ;
        knight_attacks |= (knight_bitboard << 15) & (~FILE_H_BB)            ;
        knight_attacks |= (knight_bitboard <<  6) & (~(FILE_G_BB|FILE_H_BB));
        knight_attacks |= (knight_bitboard >> 10) & (~(FILE_G_BB|FILE_H_BB));
        knight_attacks |= (knight_bitboard >> 17) & (~FILE_H_BB)            ;

        return knight_attacks;
    }

    BitBoard get_bishop_attacks(Square square, BitBoard occupancy_bitboard)
    {
        const Magic::Info& bishop_info = Magic::bishop_info_table_[square];
        occupancy_bitboard &= bishop_info.occupancy_mask;
        occupancy_bitboard *= bishop_info.magic;
        occupancy_bitboard >>= (N_SQUARES - bishop_info.bit_index);
        return bishop_info.attack_table[occupancy_bitboard];
    }

    BitBoard get_rook_attacks(Square square, BitBoard occupancy_bitboard)
    {
        const Magic::Info& rook_info = Magic::rook_info_table_[square];
        occupancy_bitboard &= rook_info.occupancy_mask;
        occupancy_bitboard *= rook_info.magic;
        occupancy_bitboard >>= (N_SQUARES - rook_info.bit_index);
        return rook_info.attack_table[occupancy_bitboard];
    }

    BitBoard get_queen_attacks(Square square, BitBoard occupancy_bitboard)
    {
        return get_bishop_attacks(square, occupancy_bitboard) | get_rook_attacks(square, occupancy_bitboard);
    }

    BitBoard gen_king_attacks_(Square square)
    {
        BitBoard king_bitboard = BitBoards::square_set_in(square);
        BitBoard king_attacks = EMPTY_BB;

        king_attacks |= BitBoards::step<NORTH>       (king_bitboard);
        king_attacks |= BitBoards::step<NORTH_WEST>  (king_bitboard);
        king_attacks |= BitBoards::step<WEST>        (king_bitboard);
        king_attacks |= BitBoards::step<SOUTH_WEST>  (king_bitboard);
        king_attacks |= BitBoards::step<SOUTH>       (king_bitboard);
        king_attacks |= BitBoards::step<SOUTH_EAST>  (king_bitboard);
        king_attacks |= BitBoards::step<EAST>        (king_bitboard);
        king_attacks |= BitBoards::step<NORTH_EAST>  (king_bitboard);

        return king_attacks;
    }



    void init_knight_attacks_()
    {
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            knight_attacks_[square] = gen_knight_attacks_(square);
        }
    }

    void init_king_attacks_()
    {
        for(Square square = SQ_A1; square < N_SQUARES; ++square)
        {
            king_attacks_[square] = gen_king_attacks_(square);
        }
    }


    void init()
    {
        Magic::init_();
        
        init_knight_attacks_();
        init_king_attacks_();
    }

    BitBoard get_knight_attacks(Square square)
    {
        return knight_attacks_[square];
    }

    BitBoard get_king_attacks(Square square)
    {
        return king_attacks_[square];
    }
}