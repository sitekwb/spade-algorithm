//
// Created by Wojtek on 07/01/2022.
//

#include "../include/Tidlist.h"

Tidlist::Tidlist(const SpadeSequence& sequence) : sequence(sequence), sup(0) {}

void Tidlist::addTransaction(std::shared_ptr<Transaction> transaction) {
    if(client_transactions.contains(transaction->getClient())){
        for(auto const &trans: client_transactions[transaction->getClient()]){
            if(*transaction == *trans){
                // transaction is existing
                return;
            }
        }
        client_transactions[transaction->getClient()].push_back(transaction);
    }
    else{
        client_transactions.insert({transaction->getClient(), std::vector<std::shared_ptr<Transaction>>({transaction})});
    }
    ++sup;
}

bool Tidlist::operator==(const Tidlist &rhs) const {
    return sequence == rhs.sequence;
}

bool Tidlist::operator!=(const Tidlist &rhs) const {
    return !(rhs == *this);
}

Tidlist::Tidlist(const Item& item) : sequence({SpadeSubseq({item})}), sup(0) {}

const SpadeSequence &Tidlist::getSequence() const {
    return sequence;
}

bool Tidlist::operator==(const Item &rhs) const {
    return sequence.size() == 1 and sequence.getSubseqs()[0].getItems().size() == 1 and *(sequence.getSubseqs()[0].getItems().begin()) == rhs;
}

bool Tidlist::operator!=(const Item &rhs) const {
    return !(rhs == *this);
}

bool Tidlist::operator==(const SpadeSequence &rhs) const {
    return sequence == rhs;
}

bool Tidlist::operator!=(const SpadeSequence &rhs) const {
    return !(rhs == *this);
}

bool Tidlist::existsClient(const Client &client) const {
    return client_transactions.contains(client);
}

const std::map<Client, std::vector<std::shared_ptr<Transaction>>> &Tidlist::getClientTransactions() const {
    return client_transactions;
}

unsigned long Tidlist::getSup() const {
    return sup;
}
