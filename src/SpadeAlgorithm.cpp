//
// Created by Wojtek on 07/01/2022.
//

#include "../include/SpadeAlgorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template<class T>
using us = std::unordered_set<std::shared_ptr<T>, SPHash<T>, SPComparator<T>>;


void SpadeAlgorithm::fetchData(std::string &filename) {
    ifstream ifs(filename);

    for (std::string line; std::getline(ifs, line);) {
        std::istringstream iss(line);
        int cid, tid;
        iss >> cid >> tid;
        auto client = getOrCreateClient(cid);
        auto transaction = std::make_shared<Transaction>(client, tid);
        for (int n; iss >> n;) {
            auto item = getOrCreateItem(n);
            transaction->addItem(item);
            getOrCreateTidlist(std::make_shared<SpadeSequence>(item), tidlists[0])->addTransaction(transaction);
        }
    }
}

std::optional<std::shared_ptr<Client>> SpadeAlgorithm::findClient(int cid) {
    auto it = clients.find(std::make_shared<Client>(cid));
    if (it == clients.end()) {
        return std::nullopt;
    }
    return *it;
}

std::shared_ptr<Client> SpadeAlgorithm::getOrCreateClient(int cid) {
    auto client = findClient(cid);
    if (client.has_value()) {
        return client.value();
    }
    auto new_client = std::make_shared<Client>(cid);
    clients.insert(new_client);
    return new_client;
}

std::optional<std::shared_ptr<Item>> SpadeAlgorithm::findItem(int id) {
    auto it = items.find(std::make_shared<Item>(id));
    if (it == items.end()) {
        return std::nullopt;
    }
    return *it;
}

std::shared_ptr<Item> SpadeAlgorithm::getOrCreateItem(int id) {
    auto item = findItem(id);
    if (item.has_value()) {
        return item.value();
    }
    auto new_item = std::make_shared<Item>(id);
    items.insert(new_item);
    return new_item;
}



std::optional<std::shared_ptr<Tidlist>> SpadeAlgorithm::findSingleTidlist(const std::shared_ptr<SpadeSequence>& seq, std::vector<std::shared_ptr<Tidlist>> &tidlist_vector) {
    for(auto const &e: tidlist_vector){
        if(*e == *seq){
            return e;
        }
    }
    return std::nullopt;
}

std::shared_ptr<Tidlist> SpadeAlgorithm::getOrCreateTidlist(const std::shared_ptr<SpadeSequence>& seq, std::vector<std::shared_ptr<Tidlist>> &tidlist_vector) {
    auto existing_tidlist = findSingleTidlist(seq, tidlist_vector);
    if(existing_tidlist.has_value()){
        return existing_tidlist.value();
    }
    auto new_tidlist = std::make_shared<Tidlist>(seq);
    tidlist_vector.push_back(new_tidlist);
    return new_tidlist;
}

SpadeAlgorithm::SpadeAlgorithm(int minSup): tidlists(), minSup(minSup) {
    tidlists.emplace_back();
}

void SpadeAlgorithm::iterate() {
    std::vector<std::shared_ptr<Tidlist>> ust;
    for(auto e_it = tidlists.back().begin(); e_it != tidlists.back().end(); e_it++){
        auto &e = *e_it;
        auto f_it = e_it;
        for(++f_it; f_it != tidlists.back().end(); ++f_it) {
            auto &f = *f_it;
            auto res = e->getSequence()->compare(f->getSequence());
            auto similarity = res.first;
            auto equality = res.second;
            if (similarity == Similarity::SINGLE and equality == Equality::EQUAL) {
                auto new_seq = std::make_shared<SpadeSequence>(e->getSequence());
                auto last_subseq = *(new_seq->getSubseqs().end() - 1);
                new_seq->addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::MORE, e, f, tidlist);
            } else if (similarity == Similarity::SINGLE and equality == Equality::NON_EQUAL) {
                auto new_seq = std::make_shared<SpadeSequence>(e->getSequence()->getSubseqs());
                auto last_subseq = *(f->getSequence()->getSubseqs().end() - 1);
                new_seq->addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::MORE, e, f, tidlist);

                new_seq = std::make_shared<SpadeSequence>(e->getSequence());
                new_seq->addSubseqBeforeLast(last_subseq);
                tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::LESS, e, f, tidlist);

                new_seq = std::make_shared<SpadeSequence>(e->getSequence());
                new_seq->addItemsToLastSubseq(last_subseq);
                tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::EQ, e, f, tidlist);
            } else if (similarity == Similarity::PLURAL and equality == Equality::NON_EQUAL) {
                auto new_seq = std::make_shared<SpadeSequence>(e->getSequence());
                auto last_subseq = *(f->getSequence()->getSubseqs().end() - 1);
                new_seq->addDifferingItemToLastSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::MORE, e, f, tidlist);
            } else if (similarity == Similarity::FIRST_PLURAL) {
                auto new_seq = std::make_shared<SpadeSequence>(e->getSequence());
                auto last_subseq = *(f->getSequence()->getSubseqs().end() - 1);
                new_seq->addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::MORE, e, f, tidlist);
            } else if (similarity == Similarity::FIRST_SINGLE) {
                auto new_seq = std::make_shared<SpadeSequence>(f->getSequence());
                auto last_subseq = *(e->getSequence()->getSubseqs().end() - 1);
                new_seq->addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                findTransactions(AddTransactionWhen::LESS, e, f, tidlist);
            }
        }
    }
    // get or create tidlist instead of creating new
    for(auto it = ust.begin(); it != ust.end();){
        if((*it)->getSup() <= minSup){
            it = ust.erase(it);
        }
        else{
            ++it;
        }
    }
    tidlists.emplace_back(ust);
}

void SpadeAlgorithm::findTransactions(AddTransactionWhen addTransactionWhen, std::shared_ptr<Tidlist> const &e,
                                      std::shared_ptr<Tidlist> const &f, std::shared_ptr<Tidlist> &tidlist) {
    for(auto &cl_trans: e->getTransactions()){
        auto client = cl_trans.first;
        auto const &e_transactions = cl_trans.second;
        if(f->existsClient(*client)){
            auto const &f_transactions = f->getTransactions().at(client);
            for(auto const &et: e_transactions) {
                for(auto const &ft: f_transactions) {
                    if(addTransactionWhen == AddTransactionWhen::EQ and *ft == *et){
                        tidlist->addTransaction(ft);
                    }
                    else if(addTransactionWhen == AddTransactionWhen::LESS and *ft < *et){
                        tidlist->addTransaction(ft);
                    }
                    else if(addTransactionWhen == AddTransactionWhen::MORE and *ft > *et){
                        tidlist->addTransaction(ft);
                    }
                }
            }
        }
    }
}

void SpadeAlgorithm::run() {
    while(getLastTidlistSetSize() > 0){
        iterate();
    }
}

size_t SpadeAlgorithm::getLastTidlistSetSize() const {
    return tidlists.back().size();
}




