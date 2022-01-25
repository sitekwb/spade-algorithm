#include <iostream>
#include "include/SpadeAlgorithm.h"

int main() {

    auto alg = SpadeAlgorithm(0);
    std::string filename = "../file.txt";

    uset<SpadeSequence> x;
    x.insert(std::make_shared<SpadeSequence>(std::make_shared<SpadeSubseq>(std::make_shared<Item>(1))));
    x.insert(std::make_shared<SpadeSequence>(std::make_shared<SpadeSubseq>(std::make_shared<Item>(2))));
    x.insert(std::make_shared<SpadeSequence>(std::make_shared<SpadeSubseq>(std::make_shared<Item>(3))));
    x.insert(std::make_shared<SpadeSequence>(std::make_shared<SpadeSubseq>(std::make_shared<Item>(1))));


    alg.fetchData(filename);
    alg.run();

    std::cout <<std::endl <<"Hello, World!" << std::endl;
    return 0;
}
