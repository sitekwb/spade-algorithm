//
// Created by Wojtek on 07/01/2022.
//

#include "../include/Item.h"

Item::Item(int id) : id(id) {}

int Item::getId() const {
    return id;
}

bool Item::operator==(const Item &rhs) const {
    return id == rhs.id;
}

bool Item::operator!=(const Item &rhs) const {
    return !(rhs == *this);
}
