//
// Created by Wojtek on 07/01/2022.
//

#include "../include/Transaction.h"

Transaction::Transaction(std::shared_ptr<Client> client, int time) : time(time), client(std::move(client)) {}

void Transaction::addItem(std::shared_ptr<Item> item) {
    items.push_back(item);
}

int Transaction::getTime() const {
    return time;
}

const std::shared_ptr<Client> &Transaction::getClient() const {
    return client;
}

bool Transaction::operator<(const Transaction &rhs) const {
    if(client != rhs.client){
        return false;
    }
    return time < rhs.time;
}

bool Transaction::operator>(const Transaction &rhs) const {
    return rhs < *this;
}

bool Transaction::operator<=(const Transaction &rhs) const {
    return !(rhs < *this);
}

bool Transaction::operator>=(const Transaction &rhs) const {
    return !(*this < rhs);
}

bool Transaction::operator==(const Transaction &rhs) const {
    return time == rhs.time &&
           client == rhs.client;
}

bool Transaction::operator!=(const Transaction &rhs) const {
    return !(rhs == *this);
}

