//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_TIDLIST_H
#define SPADE_ALGORITHM_TIDLIST_H


#include "Transaction.h"
#include "SpadeSequence.h"
#include <vector>
#include <functional>
#include <map>

class Tidlist {
    SpadeSequence sequence;
    std::map<Client, std::vector<std::shared_ptr<Transaction>>> client_transactions;
    unsigned long sup;
public:
    explicit Tidlist(const SpadeSequence& sequence);

    explicit Tidlist(const Item& item);

    void addTransaction(std::shared_ptr<Transaction> transaction);

    [[nodiscard]] unsigned long getSup() const;

    [[nodiscard]] const SpadeSequence &getSequence() const;

    bool operator==(const Item &rhs) const;

    bool operator!=(const Item &rhs) const;

    bool operator==(const SpadeSequence &rhs) const;

    bool operator!=(const SpadeSequence &rhs) const;

    bool operator==(const Tidlist &rhs) const;

    bool operator!=(const Tidlist &rhs) const;

    [[nodiscard]] bool existsClient(const Client &client) const;

    [[nodiscard]] const std::map<Client, std::vector<std::shared_ptr<Transaction>>> &getClientTransactions() const;
};

//
//template<>
//struct std::hash<Tidlist>
//{
//    std::size_t operator()(Tidlist const& t) const noexcept
//    {
//        return std::hash<SpadeSequence>{}(t.getSequence());
//    }
//};



#endif //SPADE_ALGORITHM_TIDLIST_H
