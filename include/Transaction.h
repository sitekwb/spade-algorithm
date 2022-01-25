//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_TRANSACTION_H
#define SPADE_ALGORITHM_TRANSACTION_H

#include "Item.h"
#include "Client.h"
#include <vector>
#include <unordered_set>

class Transaction {
    std::unordered_set<Item> items;
    int time;
    Client client;
public:
    Transaction(const Client& client, int time);
    void addItem(const Item& item);

    [[nodiscard]] int getTime() const;

    [[nodiscard]] const Client &getClient() const;

    bool operator==(const Transaction &rhs) const;

    bool operator!=(const Transaction &rhs) const;

    bool operator<(const Transaction &rhs) const;

    bool operator>(const Transaction &rhs) const;

    bool operator<=(const Transaction &rhs) const;

    bool operator>=(const Transaction &rhs) const;
};
//
//template<>
//struct std::hash<Transaction>
//{
//    std::size_t operator()(Transaction const& t) const noexcept
//    {
//        return std::hash<int>{}(t.getClient()) + std::hash<int>{}(t.getTime());
//    }
//};



#endif //SPADE_ALGORITHM_TRANSACTION_H
