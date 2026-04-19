#include "transpositiontable.hpp"

#include "utils.hpp"

#include <stdexcept>
#include <algorithm>

TranspositionTable::TranspositionTable(uint64_t size) : size_{size} 
{
    if(!is_power_of_two(size))
        throw std::runtime_error("TT only supports size of power of two");
    table_.resize(size_);
    hash_mask_ = std::numeric_limits<uint64_t>::max();
    hash_mask_ >>= 64 - LSB(size);
}

void TranspositionTable::insert(const TTEntry& tt_entry)
{
    auto& bucket = table_[get_partial_key(tt_entry.key)];
    for(size_t i = 0; i < BUCKET_SIZE; ++i)
    {
        auto& bucket_entry = bucket[i];
        // Depth preferred policy for tier-2 collisions
        if(bucket_entry.key == tt_entry.key)
        {
            if(tt_entry.depth < bucket_entry.depth)
                return;
            bucket_entry = tt_entry;
            bucket_entry.is_end = false;
            return;
        }
        if(bucket_entry.is_end)
        {
            bucket_entry = tt_entry;
            bucket_entry.is_end = false;
            bucket[(i + 1) % BUCKET_SIZE].is_end = true;
            return;
        }
    }
    // LRU policy when bucket is full
    bucket[0] = tt_entry;
    bucket[0].is_end = false;
    bucket[(BUCKET_SIZE + 1) % BUCKET_SIZE].is_end = true;
}

void TranspositionTable::clear()
{
    std::ranges::fill(table_, TTBucket{});
}

std::optional<TTEntry> TranspositionTable::probe(Zobrist::HashKey key) const
{
    const auto& bucket = table_[get_partial_key(key)];
    for(const auto& entry : bucket)
    {
        if(entry.key == key)
            return entry;
    }
    return std::nullopt;
}

uint64_t TranspositionTable::get_partial_key(Zobrist::HashKey key) const
{
    return hash_mask_ & key;
}