//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_ITEM_H
#define SPADE_ALGORITHM_ITEM_H

#include <functional>
#include <ostream>
#include <map>

class Item {
    int id;
public:
    explicit Item(int id);

    [[nodiscard]] int getId() const;

    bool operator==(const Item &rhs) const;

    bool operator!=(const Item &rhs) const;

    bool operator<(const Item &rhs) const;

    bool operator>(const Item &rhs) const;

    bool operator<=(const Item &rhs) const;

    bool operator>=(const Item &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Item &item);

    inline static std::map<Item, std::string> item_dictionary;
};


template<>
struct std::hash<Item>
{
    std::size_t operator()(Item const& c) const noexcept
    {
        return std::hash<int>{}(c.getId());
    }
};

#endif //SPADE_ALGORITHM_ITEM_H
