//
// Created by Wojtek on 07/01/2022.
//

#include "../include/Tidlist.h"

Tidlist::Tidlist(std::shared_ptr<SpadeSequence> sequence) : sequence(sequence) {}

void Tidlist::addTransaction(std::shared_ptr<Transaction> transaction) {
    if(transactions.contains(transaction->getClient())){
        transactions[transaction->getClient()].push_back(transaction);
    }
    transactions.insert({transaction->getClient(), std::vector<std::shared_ptr<Transaction>>({transaction})});
}

bool Tidlist::operator==(const Tidlist &rhs) const {
    return sequence == rhs.sequence;
}

bool Tidlist::operator!=(const Tidlist &rhs) const {
    return !(rhs == *this);
}

void Tidlist::setSequence(std::shared_ptr<SpadeSequence> sequence) {
    Tidlist::sequence = sequence;
}

Tidlist::Tidlist(std::shared_ptr<Item> item) : sequence(std::make_shared<SpadeSequence>(std::make_shared<SpadeSubseq>(item))) {

}

const std::shared_ptr<SpadeSequence> &Tidlist::getSequence() const {
    return sequence;
}

unsigned long Tidlist::getSup() const {
    return transactions.size();
}

const std::map<std::shared_ptr<Client>, std::vector<std::shared_ptr<Transaction>>> &Tidlist::getTransactions() const {
    return transactions;
}

bool Tidlist::operator==(const Item &rhs) const {
    return sequence->getSubseqs().size() == 1 and sequence->getSubseqs()[0]->getItems().size() == 1 and **(sequence->getSubseqs()[0]->getItems().begin()) == rhs;
}

bool Tidlist::operator!=(const Item &rhs) const {
    return !(rhs == *this);
}

bool Tidlist::operator==(const SpadeSequence &rhs) const {
    return *sequence == rhs;
}

bool Tidlist::operator!=(const SpadeSequence &rhs) const {
    return !(rhs == *this);
}

bool Tidlist::existsClient(const Client &client) const {
    for(auto const &c: transactions){
        if(*(c.first) == client){
            return true;
        }
    }
    return false;
}
