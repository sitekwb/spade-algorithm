//
// Created by Wojtek on 07/01/2022.
//

#include "../include/SpadeAlgorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


void SpadeAlgorithm::fetchData(std::string &filename, std::optional<float> minSupFrac) {
    std::cout << "Fetching data" << std::endl;
    ifstream ifs(filename);

    std::vector<std::shared_ptr<Tidlist>> ust;
    lines = 0;
    for (std::string line; std::getline(ifs, line) and lines < maxLines; ++lines) {
        if(lines % 1000 == 0) {
            std::cout << "Fetching line " << lines << std::endl;
        }
        std::istringstream iss(line);
        int cid, tid;
        iss >> cid >> tid;
        auto client = Client(cid);
        auto transaction = std::make_shared<Transaction>(client, tid);
        for (int n; iss >> n;) {
            auto item = Item(n);
            transaction->addItem(item);
            getOrCreateTidlist(SpadeSequence(item), ust)->addTransaction(transaction);
        }
    }
    if(minSupFrac.has_value()) {
        setRelationalMinSup(minSupFrac.value());
    }
    std::cout << "Removing not frequent" << std::endl;
    removeIfNotFrequent(ust);
    tidlists.push_back(ust);
}


void SpadeAlgorithm::fetchDictionary(const string &dictionary_filename) {
    ifstream ifs(dictionary_filename);

    for (std::string line; std::getline(ifs, line);) {
        std::istringstream iss(line);
        int item_id;
        std::string text;
        iss >> item_id >> text;
        Item::item_dictionary.insert({Item(item_id), text});
    }
}

SpadeAlgorithm::SpadeAlgorithm(int minSup, int maxLines): tidlists(), minSup(minSup), maxLines(maxLines), lines(0) {
}

void SpadeAlgorithm::iterate() {
    std::vector<std::shared_ptr<Tidlist>> ust;
    std::cout << tidlists.back().size() << " tidlists to process..." << std::endl;
    int i = 1;
    for(auto e_it = tidlists.back().begin(); e_it != tidlists.back().end(); e_it++, i++){
        if(i % 100 == 0) {
            std::cout << "Processing tidlist number " << i << "/" << tidlists.back().size() << std::endl;
        }
        auto &e = *e_it;
        auto f_it = e_it;
        for(++f_it; f_it != tidlists.back().end(); ++f_it) {
            auto &f = *f_it;
            auto res = e->getSequence().compare(f->getSequence());
            auto similarity = res.first;
            auto equality = res.second;
            if (similarity == Similarity::SINGLE and equality == Equality::EQUAL) {
                auto new_seq = SpadeSequence(e->getSequence());
                auto last_subseq = *(new_seq.getSubseqs().end() - 1);
                new_seq.addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::MORE, e, f, tidlist);
            } else if (similarity == Similarity::SINGLE and equality == Equality::NON_EQUAL) {
                auto new_seq = SpadeSequence(e->getSequence());
                auto last_subseq = *(f->getSequence().getSubseqs().end() - 1);
                new_seq.addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::MORE, e, f, tidlist);

                new_seq = SpadeSequence(e->getSequence());
                new_seq.addSubseqBeforeLast(last_subseq);
                tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::LESS, e, f, tidlist);

                new_seq = SpadeSequence(e->getSequence());
                new_seq.addItemsToLastSubseq(last_subseq);
                tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::EQ, e, f, tidlist);
            } else if (similarity == Similarity::PLURAL and equality == Equality::NON_EQUAL) {
                auto new_seq = SpadeSequence(e->getSequence());
                auto last_subseq = *(f->getSequence().getSubseqs().end() - 1);
                new_seq.addDifferingItemToLastSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::MORE, e, f, tidlist);
            } else if (similarity == Similarity::FIRST_PLURAL) {
                auto new_seq = SpadeSequence(e->getSequence());
                auto last_subseq = *(f->getSequence().getSubseqs().end() - 1);
                new_seq.addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::MORE, e, f, tidlist);
            } else if (similarity == Similarity::FIRST_SINGLE) {
                auto new_seq = SpadeSequence(f->getSequence());
                auto last_subseq = *(e->getSequence().getSubseqs().end() - 1);
                new_seq.addSubseq(last_subseq);
                auto tidlist = getOrCreateTidlist(new_seq, ust);
                addCorrespondingTransactions(AddTransactionWhen::LESS, e, f, tidlist);
            }
        }
    }
    removeIfNotFrequent(ust);
    tidlists.emplace_back(ust);
}



void SpadeAlgorithm::addCorrespondingTransactions(AddTransactionWhen addTransactionWhen, const std::shared_ptr<Tidlist> &e, const std::shared_ptr<Tidlist> &f, std::shared_ptr<Tidlist> &tidlist) {
    for(auto &cl_trans: e->getClientTransactions()){
        auto client = cl_trans.first;
        auto const &e_transactions = cl_trans.second;
        if(f->existsClient(client)){
            auto const &f_transactions = f->getClientTransactions().at(client);
            for(auto const &et: e_transactions) {
                for(auto const &ft: f_transactions) {
                    if(addTransactionWhen == AddTransactionWhen::EQ and *ft == *et){
                        tidlist->addTransaction(ft);
                    }
                    else if(addTransactionWhen == AddTransactionWhen::LESS and *ft < *et){
                        tidlist->addTransaction(et);
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
    int i = 2;
    while(getLastTidlistSetSize() > 0){
        std::cout << "Processing tidlists of size " << i++ << std::endl;
        iterate();
    }
}

size_t SpadeAlgorithm::getLastTidlistSetSize() const {
    return tidlists.back().size();
}

std::shared_ptr<Tidlist> SpadeAlgorithm::getOrCreateTidlist(const SpadeSequence &seq, vector<std::shared_ptr<Tidlist>> &tidlist_vector) {
    auto existing_tidlist = findTidlist(seq, tidlist_vector);
    if(existing_tidlist.has_value()){
        return existing_tidlist.value();
    }
    auto new_tidlist = std::make_shared<Tidlist>(seq);
    tidlist_vector.push_back(new_tidlist);
    return new_tidlist;
}

std::optional<std::shared_ptr<Tidlist>> SpadeAlgorithm::findTidlist(const SpadeSequence &seq, const vector<std::shared_ptr<Tidlist>> &tidlist_vector) {
    for(auto const &e: tidlist_vector){
        if(*e == seq){
            return e;
        }
    }
    return std::nullopt;
}

ostream &operator<<(ostream &os, const SpadeAlgorithm &algorithm) {
    os << "MinSup = " << algorithm.minSup << std::endl;
    os << "MaxLines = " << algorithm.maxLines << std::endl;
    for(auto const &same_len_tidlists: algorithm.tidlists){
        for(auto const &tidlist: same_len_tidlists){
            os << tidlist->getSup() << ' ' << tidlist->getSequence() << std::endl;
        }
    }
    return os;
}

void SpadeAlgorithm::removeIfNotFrequent(vector<std::shared_ptr<Tidlist>> &ust) {
    for(auto it = ust.begin(); it != ust.end();){
        // remove if not min sup
        if((*it)->getSup() <= minSup){
            it = ust.erase(it);
        }
        else{
            ++it;
        }
    }
}

void SpadeAlgorithm::reset() {
    tidlists.clear();
    lines = 0;
}

void SpadeAlgorithm::setRelationalMinSup(float frac) {
    minSup = (int) (frac * (float)lines);
    std::cout << "Relational minSup = " << minSup << std::endl;
    std::cout << "Lines = " << lines << std::endl;
}





