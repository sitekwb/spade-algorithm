//
// Created by Wojtek on 15/01/2022.
//

#ifndef SPADE_ALGORITHM_SPADESUBSEQ_H
#define SPADE_ALGORITHM_SPADESUBSEQ_H


#include <vector>
#include "Item.h"
//#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <ostream>

class SpadeSubseq {
    std::unordered_set<Item> items;
public:
    SpadeSubseq(const std::initializer_list<Item> &items);
    SpadeSubseq(const SpadeSubseq &rhs);

    bool operator==(const SpadeSubseq &rhs) const;

    bool operator!=(const SpadeSubseq &rhs) const;

    [[nodiscard]] bool differsByOne(const SpadeSubseq &rhs) const;

    [[nodiscard]] std::optional<Item> lacksOne(const SpadeSubseq &rhs) const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool existsItem(const Item &item) const;

    [[nodiscard]] const std::unordered_set<Item> &getItems() const;

    void addItem(const Item& item);

    void addDifferingItem(const SpadeSubseq &subseq);

    friend std::ostream &operator<<(std::ostream &os, const SpadeSubseq &subseq);
};

//
//template<>
//struct std::hash<SpadeSubseq>
//{
//    std::size_t operator()(SpadeSubseq const& c) const noexcept
//    {
//        return boost::hash_range(c.getItems().begin(), c.getItems().end());
//    }
//};

#endif //SPADE_ALGORITHM_SPADESUBSEQ_H
