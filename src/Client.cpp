//
// Created by Wojtek on 07/01/2022.
//

#include <functional>
#include "../include/Client.h"

bool Client::operator==(const Client &rhs) const {
    return id == rhs.id;
}

bool Client::operator!=(const Client &rhs) const {
    return !(rhs == *this);
}

bool Client::operator==(const int &rhs) const {
    return id == rhs;
}

bool Client::operator!=(const int &rhs) const {
    return !(rhs == *this);
}

bool Client::operator<(const Client &rhs) const {
    return id < rhs.id;
}

bool Client::operator>(const Client &rhs) const {
    return rhs < *this;
}

bool Client::operator<=(const Client &rhs) const {
    return !(rhs < *this);
}

bool Client::operator>=(const Client &rhs) const {
    return !(*this < rhs);
}

Client::Client(int id) : id(id) {}

int Client::getId() const {
    return id;
}
