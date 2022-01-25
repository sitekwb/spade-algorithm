//
// Created by Wojtek on 07/01/2022.
//

#include "../include/SpadeSequence.h"
#include <boost/range/combine.hpp>

SpadeSequence::SpadeSequence(const std::vector<std::shared_ptr<SpadeSubseq>> &rhs) {
    for(auto const &e: rhs){
        subseqs.push_back(std::make_shared<SpadeSubseq>(*e));
    }
}

SpadeSequence::SpadeSequence(const std::shared_ptr<SpadeSubseq> &subseq) {
    subseqs.push_back(std::make_shared<SpadeSubseq>(*subseq));
}

const std::vector<std::shared_ptr<SpadeSubseq>> &SpadeSequence::getSubseqs() const {
    return subseqs;
}

std::pair<Similarity, Equality> SpadeSequence::compare(const std::shared_ptr<SpadeSequence> &seq) {
    Similarity similarity = Similarity::SINGLE;
    Equality equality = Equality::EQUAL;
    bool differsByOne = false;
    int size_differ = (int) this->size() - (int) seq->size();
    if (size_differ > 1 || size_differ < -1) {
        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
    }
    auto subseqs1 = this->getSubseqs();
    auto subseqs2 = seq->getSubseqs();
    auto it1 = subseqs1.begin(), it2 = subseqs2.begin();
    for (; it1 != subseqs1.end() and it2 != subseqs2.end(); it1++, it2++) {
        auto s1 = *it1, s2 = *it2;
        if (*s1 != *s2) {
            if (it1 + 1 != subseqs1.end() and it2 + 1 != subseqs2.end()) {
                // they differ in the middle so they are different
                return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
            }
            // at least one of them is last
            if (s1->differsByOne(s2)) {
                if (size_differ != 0) {
                    // they differ by one and there is something more
                    return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                }
                // both of them are last
                if (s1->size() == 1) {
                    return std::make_pair(Similarity::SINGLE, Equality::NON_EQUAL);
                } else {
                    return std::make_pair(Similarity::PLURAL, Equality::NON_EQUAL);
                }
            } else {
                auto lacking_item = s1->lacksOne(s2);
                if (lacking_item.has_value()) {
                    if (size_differ != 1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (P)()     (Px)
                    if ((it1 + 1)->get()->size() != 1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (P)(?)     (Px)
                    if (*((it1 + 1)->get()->getItems().begin()) == lacking_item.value()) {
                        return std::make_pair(Similarity::FIRST_SINGLE, Equality::EQUAL);
                    } else {
                        return std::make_pair(Similarity::FIRST_SINGLE, Equality::NON_EQUAL);
                    }
                }
                lacking_item = s2->lacksOne(s1);
                if (lacking_item.has_value()) {
                    if (size_differ != -1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (Px)     (P)(?)
                    if ((it2 + 1)->get()->size() != 1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (Px)     (P)(y/x)
                    if (*((it2 + 1)->get()->getItems().begin()) == lacking_item.value()) {
                        return std::make_pair(Similarity::FIRST_PLURAL, Equality::EQUAL);
                    } else {
                        return std::make_pair(Similarity::FIRST_PLURAL, Equality::NON_EQUAL);
                    }
                }
            }
        } else if (it1 + 1 == subseqs1.end() and it2 + 1 == subseqs2.end()) {
            // both are last
            if ((*it1)->size() == 1) {
                return std::make_pair(Similarity::SINGLE, Equality::EQUAL);
            } else {
                return std::make_pair(Similarity::PLURAL, Equality::EQUAL);
            }
        }
    }
    return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
}


size_t SpadeSequence::size() {
    return subseqs.size();
}

SpadeSequence::SpadeSequence(const SpadeSequence &rhs) {
    for(auto e: rhs.getSubseqs()){
        subseqs.push_back(e);
    }
}

void SpadeSequence::addSubseq(std::shared_ptr<SpadeSubseq> e) {
    subseqs.push_back(e);
}

void SpadeSequence::addSubseqBeforeLast(std::shared_ptr<SpadeSubseq> e) {
    subseqs.insert(--subseqs.end(), e);
}

void SpadeSequence::addItemsToLastSubseq(std::shared_ptr<SpadeSubseq>const & subseq) {
    for(auto const &e: subseq->getItems()){
        subseqs.back()->addItem(e);
    }
}

void SpadeSequence::addDifferingItemToLastSubseq(const std::shared_ptr<SpadeSubseq> &e) {
    subseqs.back()->addDifferingItem(e);
}

bool SpadeSequence::operator==(const SpadeSequence &rhs) const {
    if(subseqs.size() != rhs.getSubseqs().size()){
        return false;
    }
    auto it1 = this->subseqs.begin();
    auto it2 = rhs.getSubseqs().begin();
    for(;it1 != subseqs.end() and it2 != subseqs.end();it1++, it2++){
        if((*it1)->operator!=(**it2)){
            return false;
        }
    }
    return true;
}

bool SpadeSequence::operator!=(const SpadeSequence &rhs) const {
    return !(rhs == *this);
}

SpadeSequence::SpadeSequence(const std::shared_ptr<SpadeSequence>&rhs) : SpadeSequence(rhs->getSubseqs()) {

}

SpadeSequence::SpadeSequence(const std::shared_ptr<Item> &item) : SpadeSequence({std::make_shared<SpadeSubseq>(item)}) {

}
