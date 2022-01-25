//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_SPADESEQUENCE_H
#define SPADE_ALGORITHM_SPADESEQUENCE_H


#include <vector>
#include "Item.h"
#include "SpadeSubseq.h"
#include <boost/functional/hash.hpp>

enum Similarity{
    DIFFERENT,
    SINGLE,
    PLURAL,
    FIRST_PLURAL,
    FIRST_SINGLE,
};

enum Equality{
    EQUAL,
    NON_EQUAL,
};

class SpadeSequence {
    std::vector<std::shared_ptr<SpadeSubseq>> subseqs;
public:
    explicit SpadeSequence(const std::vector<std::shared_ptr<SpadeSubseq>> &subseqs);
    explicit SpadeSequence(const std::shared_ptr<SpadeSubseq> &subseq);
    explicit SpadeSequence(const std::shared_ptr<Item> &item);

    size_t size();
    std::pair<Similarity, Equality> compare(std::shared_ptr<SpadeSequence>const& seq);

    SpadeSequence(const SpadeSequence& rhs);
    explicit SpadeSequence(const std::shared_ptr<SpadeSequence>& rhs);
    [[nodiscard]] const std::vector<std::shared_ptr<SpadeSubseq>> &getSubseqs() const;
    void addSubseq(std::shared_ptr<SpadeSubseq> e);

    void addSubseqBeforeLast(std::shared_ptr<SpadeSubseq> e);

    void addItemsToLastSubseq(std::shared_ptr<SpadeSubseq>const & e);

    void addDifferingItemToLastSubseq(std::shared_ptr<SpadeSubseq>const & e);

    bool operator==(const SpadeSequence &rhs) const;

    bool operator!=(const SpadeSequence &rhs) const;
};


template<>
struct std::hash<SpadeSequence>
{
    std::size_t operator()(SpadeSequence const& c) const noexcept
    {
        size_t hash_value = 0;
        for(auto &e: c.getSubseqs()){
            hash_value += std::hash<SpadeSubseq>{}(*e);
        }
        return hash_value;
    }
};


#endif //SPADE_ALGORITHM_SPADESEQUENCE_H
