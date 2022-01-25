//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_TIDLIST_H
#define SPADE_ALGORITHM_TIDLIST_H


#include "Transaction.h"
#include "SpadeSequence.h"
#include <vector>
#include <functional>

class Tidlist {
    std::shared_ptr<SpadeSequence> sequence;
    std::map<std::shared_ptr<Client>, std::vector<std::shared_ptr<Transaction>>> transactions;
public:
    explicit Tidlist(std::shared_ptr<SpadeSequence> sequence);

    [[nodiscard]] unsigned long getSup() const;

    explicit Tidlist(std::shared_ptr<Item> item);

    void setSequence(std::shared_ptr<SpadeSequence> sequence);

    void addTransaction(std::shared_ptr<Transaction> transaction);

    [[nodiscard]] const std::shared_ptr<SpadeSequence> &getSequence() const;

    bool operator==(const Item &rhs) const;

    bool operator!=(const Item &rhs) const;

    bool operator==(const SpadeSequence &rhs) const;

    bool operator!=(const SpadeSequence &rhs) const;

    bool operator==(const Tidlist &rhs) const;

    bool operator!=(const Tidlist &rhs) const;

    bool existsClient(const Client &client) const;

    [[nodiscard]] const std::map<std::shared_ptr<Client>, std::vector<std::shared_ptr<Transaction>>> &getTransactions() const;
};


template<>
struct std::hash<Tidlist>
{
    std::size_t operator()(Tidlist const& t) const noexcept
    {
        return std::hash<SpadeSequence>{}(*t.getSequence());
    }
};



#endif //SPADE_ALGORITHM_TIDLIST_H
