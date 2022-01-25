//
// Created by Wojtek on 07/01/2022.
//

#include "../include/Transaction.h"

Transaction::Transaction(const Client& client, int time) : time(time), client(client) {}

void Transaction::addItem(const Item& item) {
    items.insert(item);
}

int Transaction::getTime() const {
    return time;
}

const Client& Transaction::getClient() const {
    return client;
}

bool Transaction::operator<(const Transaction &rhs) const {
    return client == rhs.client && time < rhs.time;
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

