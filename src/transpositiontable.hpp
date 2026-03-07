#ifndef ALTAIR_TRANSPOSITIONTABLE_HPP
#define ALTAIR_TRANSPOSITIONTABLE_HPP

#include "move.hpp"
#include "zobrist.hpp"

#include <vector>
#include <optional>
#include <boost/container/small_vector.hpp>

constexpr unsigned long long operator"" _K(unsigned long long x) {
    return x * 1024;
}

constexpr unsigned long long operator"" _M(unsigned long long x) {
    return x * 1024 * 1024;
}

constexpr size_t BUCKET_SIZE = 16;

struct TTEntry
{
    enum class NodeType
    {
        EXACT,
        UPPERBOUND,
        LOWERBOUND
    };
    Zobrist::HashKey key = 0;
    Move best_move;
    uint32_t depth = 0;
    uint32_t score = 0;
    NodeType node_type;
};

using TTBucket = boost::container::small_vector<TTEntry, BUCKET_SIZE>;

class TranspostionTable 
{
    public:
    TranspostionTable(uint64_t size);
    void insert(TTEntry&& tt_entry);
    std::optional<TTEntry> probe(Zobrist::HashKey key) const;

    private:
    uint64_t get_partial_key(Zobrist::HashKey key) const;

    private:
    uint64_t size_;
    uint64_t hash_mask_;
    std::vector<TTBucket> table_;
};

#endif // ALTAIR_TRANSPOSITIONTABLE_HPP