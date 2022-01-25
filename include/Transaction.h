//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_TRANSACTION_H
#define SPADE_ALGORITHM_TRANSACTION_H

#include "Item.h"
#include "Client.h"
#include <vector>
#include <boost/functional/hash.hpp>

class Transaction {
    std::vector<std::shared_ptr<Item>> items;
    int time;
    std::shared_ptr<Client> client;
public:
    Transaction(std::shared_ptr<Client> client, int time);
    void addItem(std::shared_ptr<Item> item);

    [[nodiscard]] int getTime() const;

    [[nodiscard]] const std::shared_ptr<Client> &getClient() const;

    bool operator==(const Transaction &rhs) const;

    bool operator!=(const Transaction &rhs) const;

    bool operator<(const Transaction &rhs) const;

    bool operator>(const Transaction &rhs) const;

    bool operator<=(const Transaction &rhs) const;

    bool operator>=(const Transaction &rhs) const;
};

template<>
struct std::hash<Transaction>
{
    std::size_t operator()(Transaction const& t) const noexcept
    {
        return boost::hash_value(std::make_pair(t.getClient(), t.getTime()));
    }
};



#endif //SPADE_ALGORITHM_TRANSACTION_H
