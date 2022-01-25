//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_ITEM_H
#define SPADE_ALGORITHM_ITEM_H

#include <functional>

class Item {
    int id;
public:
    Item(int id);

    int getId() const;

    bool operator==(const Item &rhs) const;

    bool operator!=(const Item &rhs) const;
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
