//
// Created by Wojtek on 15/01/2022.
//

#ifndef SPADE_ALGORITHM_SPADESUBSEQ_H
#define SPADE_ALGORITHM_SPADESUBSEQ_H


#include <vector>
#include "Item.h"
#include <boost/functional/hash.hpp>
#include <unordered_set>

template<class T>
struct SPComparator {
    bool operator()(const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) const {
        return a->operator==(*b);
    }
};

template<class T>
struct SPHash{
    size_t operator()(const std::shared_ptr<T> &a) const{
        return std::hash<T>{}(*a);
    }
};

template<class T>
using uset = std::unordered_set<std::shared_ptr<T>, SPHash<T>, SPComparator<T>>;


class SpadeSubseq {
    uset<Item> items;
public:
    explicit SpadeSubseq(const std::unordered_set<std::shared_ptr<Item>> &items);
    explicit SpadeSubseq(const std::shared_ptr<Item> &item);
    SpadeSubseq(const SpadeSubseq &rhs);

    bool operator==(const SpadeSubseq &rhs) const;

    bool operator!=(const SpadeSubseq &rhs) const;

    [[nodiscard]] bool differsByOne(const std::shared_ptr<SpadeSubseq> &rhs) const;

    std::optional<std::shared_ptr<Item>> lacksOne(const std::shared_ptr<SpadeSubseq> &rhs) const;

    size_t size() const;

    [[nodiscard]] bool existsItem(const std::shared_ptr<Item> &item) const;

    [[nodiscard]] const uset<Item> &getItems() const;

    void addItem(std::shared_ptr<Item> const & item);

    void addDifferingItem(std::shared_ptr<SpadeSubseq> const &subseq);
};


template<>
struct std::hash<SpadeSubseq>
{
    std::size_t operator()(SpadeSubseq const& c) const noexcept
    {
        size_t hash_value = 0;
        for(auto &e: c.getItems()){
            hash_value += std::hash<Item>{}(*e);
        }
        return hash_value;
    }
};

#endif //SPADE_ALGORITHM_SPADESUBSEQ_H
