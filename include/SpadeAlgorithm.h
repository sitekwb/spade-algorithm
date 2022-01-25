//
// Created by Wojtek on 07/01/2022.
//

#ifndef SPADE_ALGORITHM_SPADEALGORITHM_H
#define SPADE_ALGORITHM_SPADEALGORITHM_H


#include <string>
#include <vector>
#include <optional>
#include <map>
#include <unordered_set>
#include "Client.h"
#include "Transaction.h"
#include "Tidlist.h"

enum AddTransactionWhen{
    LESS,
    EQ,
    MORE
};

class SpadeAlgorithm {
    int minSup;
    uset<Client> clients;
    std::vector<std::shared_ptr<Transaction>> transactions;
    uset<Item> items;
    std::vector<std::vector<std::shared_ptr<Tidlist>>> tidlists;
public:
    SpadeAlgorithm(int minSup);

    void run();
    void iterate();
    void fetchData(std::string &filename);
    std::optional<std::shared_ptr<Client>> findClient(int cid);
    std::shared_ptr<Client> getOrCreateClient(int cid);

    std::optional<std::shared_ptr<Item>> findItem(int id);
    std::shared_ptr<Item> getOrCreateItem(int id);

    std::optional<std::shared_ptr<Tidlist>> findSingleTidlist(const std::shared_ptr<SpadeSequence>& seq, std::vector<std::shared_ptr<Tidlist>> &tidlist_vector);
    std::shared_ptr<Tidlist> getOrCreateTidlist(const std::shared_ptr<SpadeSequence>& seq, std::vector<std::shared_ptr<Tidlist>> &tidlist_vector);

    [[nodiscard]] size_t getLastTidlistSetSize() const;



    void findTransactions(AddTransactionWhen addTransactionWhen, std::shared_ptr<Tidlist> const &e, std::shared_ptr<Tidlist> const &f, std::shared_ptr<Tidlist> &tidlist);
};


#endif //SPADE_ALGORITHM_SPADEALGORITHM_H
