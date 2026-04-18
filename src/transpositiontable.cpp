#include "transpositiontable.hpp"

#include "utils.hpp"

#include <stdexcept>
#include <algorithm>

TranspostionTable::TranspostionTable(uint64_t size) : size_{size} 
{
    if(!is_power_of_two(size))
        throw std::runtime_error("TT only supports size of power of two");
    table_.resize(size_);
    hash_mask_ = std::numeric_limits<uint64_t>::max();
    hash_mask_ >>= 64 - LSB(size);
}

void TranspostionTable::insert(const TTEntry& tt_entry)
{
    auto& bucket = table_[get_partial_key(tt_entry.key)];
    for(auto &bucket_entry : bucket)
    {
        // Depth preferred policy for tier-2 collisions
        if(bucket_entry.key == tt_entry.key)
        {
            if(tt_entry.depth < bucket_entry.depth)
                return;
            bucket_entry = tt_entry;
            return;
        }
    }
    // LRU policy when bucket is full
    if(bucket.size() >= BUCKET_SIZE - 1)
        bucket[0] = tt_entry;
    else
        bucket.push_back(tt_entry);
}

void TranspostionTable::clear()
{
    std::ranges::fill(table_, TTBucket{});
}

std::optional<TTEntry> TranspostionTable::probe(Zobrist::HashKey key) const
{
    const auto& bucket = table_[get_partial_key(key)];
    for(const auto& entry : bucket)
    {
        if(entry.key == key)
            return entry;
    }
    return std::nullopt;
}

uint64_t TranspostionTable::get_partial_key(Zobrist::HashKey key) const
{
    return hash_mask_ & key;
}