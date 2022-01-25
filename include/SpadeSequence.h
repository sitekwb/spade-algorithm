//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_SPADESEQUENCE_H
#define SPADE_ALGORITHM_SPADESEQUENCE_H


#include <vector>
#include <ostream>
#include "Item.h"
#include "SpadeSubseq.h"
//#include <boost/functional/hash.hpp>

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
    std::vector<SpadeSubseq> subseqs;
public:
    SpadeSequence(const std::initializer_list<SpadeSubseq> &subseqs);
    explicit SpadeSequence(const Item &item);
    SpadeSequence(const SpadeSequence& rhs);

    [[nodiscard]] size_t size() const;
    [[nodiscard]] std::pair<Similarity, Equality> compare(const SpadeSequence& seq) const;

    [[nodiscard]] const std::vector<SpadeSubseq> &getSubseqs() const;

    void addSubseq(const SpadeSubseq& e);

    void addSubseqBeforeLast(const SpadeSubseq& e);

    void addItemsToLastSubseq(const SpadeSubseq& e);

    void addDifferingItemToLastSubseq(const SpadeSubseq& e);

    bool operator==(const SpadeSequence &rhs) const;

    bool operator!=(const SpadeSequence &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const SpadeSequence &sequence);
};

//
//template<>
//struct std::hash<SpadeSequence>
//{
//    std::size_t operator()(SpadeSequence const& c) const noexcept
//    {
//        return boost::hash_range(c.getSubseqs().begin(), c.getSubseqs().end());
//    }
//};


#endif //SPADE_ALGORITHM_SPADESEQUENCE_H
