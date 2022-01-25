//
// Created by Wojtek on 07/01/2022.
//

#include "../include/SpadeSequence.h"
#include <boost/range/combine.hpp>

SpadeSequence::SpadeSequence(const std::initializer_list<SpadeSubseq> &rhs) :subseqs(rhs){}

SpadeSequence::SpadeSequence(const Item &item) :subseqs({SpadeSubseq({item})}){}


std::pair<Similarity, Equality> SpadeSequence::compare(const SpadeSequence &seq) const {
    Similarity similarity = Similarity::SINGLE;
    Equality equality = Equality::EQUAL;
    bool differsByOne = false;
    int size_differ = (int) this->size() - (int) seq.size();
    if (size_differ > 1 || size_differ < -1) {
        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
    }
    auto subseqs1 = this->getSubseqs();
    auto subseqs2 = seq.getSubseqs();
    auto it1 = subseqs1.begin(), it2 = subseqs2.begin();
    for (; it1 != subseqs1.end() and it2 != subseqs2.end(); it1++, it2++) {
        auto s1 = *it1, s2 = *it2;
        if (s1 != s2) {
            if (it1 + 1 != subseqs1.end() and it2 + 1 != subseqs2.end()) {
                // they differ in the middle so they are different
                return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
            }
            // at least one of them is last
            if (s1.differsByOne(s2)) {
                if (size_differ != 0) {
                    // they differ by one and there is something more
                    return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                }
                // both of them are last
                if (s1.size() == 1) {
                    return std::make_pair(Similarity::SINGLE, Equality::NON_EQUAL);
                } else {
                    return std::make_pair(Similarity::PLURAL, Equality::NON_EQUAL);
                }
            } else {
                auto lacking_item = s1.lacksOne(s2);
                if (lacking_item.has_value()) {
                    if (size_differ != 1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (P)()     (Px)
                    if ((it1 + 1)->size() != 1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (P)(?)     (Px)
                    if (*((it1 + 1)->getItems().begin()) == lacking_item.value()) {
                        return std::make_pair(Similarity::FIRST_SINGLE, Equality::EQUAL);
                    } else {
                        return std::make_pair(Similarity::FIRST_SINGLE, Equality::NON_EQUAL);
                    }
                }
                lacking_item = s2.lacksOne(s1);
                if (lacking_item.has_value()) {
                    if (size_differ != -1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (Px)     (P)(?)
                    if ((it2 + 1)->size() != 1) {
                        return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
                    }
                    // (Px)     (P)(y/x)
                    if (*((it2 + 1)->getItems().begin()) == lacking_item.value()) {
                        return std::make_pair(Similarity::FIRST_PLURAL, Equality::EQUAL);
                    } else {
                        return std::make_pair(Similarity::FIRST_PLURAL, Equality::NON_EQUAL);
                    }
                }
            }
        } else if (it1 + 1 == subseqs1.end() and it2 + 1 == subseqs2.end()) {
            // both are last
            if ((*it1).size() == 1) {
                return std::make_pair(Similarity::SINGLE, Equality::EQUAL);
            } else {
                return std::make_pair(Similarity::PLURAL, Equality::EQUAL);
            }
        }
    }
    return std::make_pair(Similarity::DIFFERENT, Equality::NON_EQUAL);
}


size_t SpadeSequence::size() const {
    return subseqs.size();
}

SpadeSequence::SpadeSequence(const SpadeSequence &rhs) : subseqs(rhs.getSubseqs()){}

void SpadeSequence::addSubseq(const SpadeSubseq& e) {
    subseqs.push_back(e);
}

void SpadeSequence::addSubseqBeforeLast(const SpadeSubseq& e) {
    subseqs.insert(--subseqs.end(), e);
}

void SpadeSequence::addItemsToLastSubseq(const SpadeSubseq& subseq) {
    for(auto const &e: subseq.getItems()){
        subseqs.back().addItem(e);
    }
}

void SpadeSequence::addDifferingItemToLastSubseq(const SpadeSubseq& e) {
    subseqs.back().addDifferingItem(e);
}

bool SpadeSequence::operator==(const SpadeSequence &rhs) const {
    return subseqs == rhs.subseqs;
}

bool SpadeSequence::operator!=(const SpadeSequence &rhs) const {
    return !(rhs == *this);
}

const std::vector<SpadeSubseq> &SpadeSequence::getSubseqs() const {
    return subseqs;
}

std::ostream &operator<<(std::ostream &os, const SpadeSequence &sequence) {
    for(auto const &subseq: sequence.subseqs){
        os << subseq;
    }
    return os;
}
