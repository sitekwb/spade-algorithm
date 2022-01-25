//
// Created by Wojtek on 15/01/2022.
//

#include "../include/SpadeSubseq.h"

template<class T>
using uset = std::unordered_set<std::shared_ptr<T>, SPHash<T>, SPComparator<T>>;


SpadeSubseq::SpadeSubseq(const std::unordered_set<std::shared_ptr<Item>> &rhs) {
    for(auto const &e: rhs){
        items.insert(e);
    }
}

SpadeSubseq::SpadeSubseq(const std::shared_ptr<Item> &item) {
    items.insert(item);
}

const uset<Item> &SpadeSubseq::getItems() const {
    return items;
}

bool SpadeSubseq::operator==(const SpadeSubseq &rhs) const {
    if(items.size() != rhs.getItems().size()){
        return false;
    }
    for(auto const &e: items){
        if(!rhs.getItems().contains(e)){
            return false;
        }
    }
//    auto it1 = items.begin();
//    auto it2 = rhs.getItems().begin();
//    for(;it1 != items.end() and it2 != rhs.getItems().end();it1++, it2++){
//        if((*it1)->operator!=(**it2)){
//            return false;
//        }
//    }
    return true;
}

bool SpadeSubseq::operator!=(const SpadeSubseq &rhs) const {
    return !(rhs == *this);
}

bool SpadeSubseq::differsByOne(const std::shared_ptr<SpadeSubseq> &rhs) const {
    int count_differences = 0;
    if (this->items.size() != rhs->getItems().size()) {
        return false;
    }
    for (auto const &e: this->items) {
        if (!rhs->existsItem(e)) {
            if (++count_differences > 1) {
                return false;
            }
        }
    }
    return true;
}

bool SpadeSubseq::existsItem(const std::shared_ptr<Item> &item) const {
    return items.find(item) != items.end();
}

std::optional<std::shared_ptr<Item>> SpadeSubseq::lacksOne(const std::shared_ptr<SpadeSubseq> &rhs) const {
    int subseq_size_differ = (int) this->items.size() - (int) rhs->getItems().size();
    if (subseq_size_differ != -1) {
        return std::nullopt;
    }
    int count_differences = 0;
    std::optional<std::shared_ptr<Item>> item = std::nullopt;
    for (auto const &e: rhs->getItems()) {
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

void SpadeSubseq::addItem(std::shared_ptr<Item> const & item) {
    items.insert(item);
}

void SpadeSubseq::addDifferingItem(const std::shared_ptr<SpadeSubseq> &subseq) {
    for(auto const &e: subseq->getItems()){
        if(not items.contains(e)){
            addItem(e);
        }
    }
}

SpadeSubseq::SpadeSubseq(const SpadeSubseq &rhs) : items(rhs.getItems()) {

}
