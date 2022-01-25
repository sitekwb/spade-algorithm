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
#include <ostream>
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
    std::vector<std::vector<std::shared_ptr<Tidlist>>> tidlists;
    int maxLines;
    int lines;
public:
    SpadeAlgorithm(int minSup, int maxLines = std::numeric_limits<int>::max());

    friend std::ostream &operator<<(std::ostream &os, const SpadeAlgorithm &algorithm);

    void fetchDictionary(const std::string &dictionary_filename);

    void setRelationalMinSup(float frac);

    void reset();

    void run();
    void iterate();
    void fetchData(std::string &filename, std::optional<float> minSupFrac = std::nullopt);

    static std::optional<std::shared_ptr<Tidlist>> findTidlist(const SpadeSequence& seq, const std::vector<std::shared_ptr<Tidlist>> &tidlist_vector);
    static std::shared_ptr<Tidlist> getOrCreateTidlist(const SpadeSequence& seq, std::vector<std::shared_ptr<Tidlist>> &tidlist_vector);

    void addCorrespondingTransactions(AddTransactionWhen addTransactionWhen, const std::shared_ptr<Tidlist> &e, const std::shared_ptr<Tidlist> &f, std::shared_ptr<Tidlist> &tidlist);

    void removeIfNotFrequent(std::vector<std::shared_ptr<Tidlist>> &ust);
    [[nodiscard]] size_t getLastTidlistSetSize() const;

};


#endif //SPADE_ALGORITHM_SPADEALGORITHM_H
