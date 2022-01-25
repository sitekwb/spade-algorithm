//
// Created by Wojtek on 15/01/2022.
//

#include "../include/SpadeSubseq.h"


SpadeSubseq::SpadeSubseq(const std::initializer_list<Item> &rhs) : items(rhs){}

bool SpadeSubseq::operator==(const SpadeSubseq &rhs) const {
    return items == rhs.items;
}

bool SpadeSubseq::operator!=(const SpadeSubseq &rhs) const {
    return !(rhs == *this);
}

bool SpadeSubseq::differsByOne(const SpadeSubseq &rhs) const {
    int count_differences = 0;
    if (this->size() != rhs.size()) {
        return false;
    }
    for (auto const &e: this->items) {
        if (!rhs.existsItem(e)) {
            if (++count_differences > 1) {
                return false;
            }
        }
    }
    return count_differences == 1;
}

bool SpadeSubseq::existsItem(const Item &item) const {
    return items.find(item) != items.end();
}

std::optional<Item> SpadeSubseq::lacksOne(const SpadeSubseq &rhs) const {
    int subseq_size_differ = (int) this->size() - (int) rhs.size();
    if (subseq_size_differ != -1) {
        return std::nullopt;
    }
    int count_differences = 0;
    std::optional<Item> item = std::nullopt;
    for (auto const &e: rhs.getItems()) {
        if (!this->existsItem(e)) {
            if(++count_differences > 1){
                return std::nullopt;
            }
            item = e;
        }
    }
    return item;
}

size_t SpadeSubseq::size() const {
    return items.size();
}

void SpadeSubseq::addItem(const Item & item) {
    items.insert(item);
}

void SpadeSubseq::addDifferingItem(const SpadeSubseq &subseq) {
    for(auto const &e: subseq.getItems()){
        if(not items.contains(e)){
            addItem(e);
            break;
        }
    }
}

SpadeSubseq::SpadeSubseq(const SpadeSubseq &rhs) : items(rhs.getItems()) {

}

const std::unordered_set<Item> &SpadeSubseq::getItems() const {
    return items;
}

std::ostream &operator<<(std::ostream &os, const SpadeSubseq &subseq) {
    os << "(";
    bool first = true;
    for(auto const &item: subseq.items) {
        if(!first){
            os << ",";
        }
        first = false;
        os << item;
    }
    os << ")";
    return os;
}
