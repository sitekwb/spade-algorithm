#include <iostream>
#include "include/SpadeAlgorithm.h"
#include <fstream>
#include <filesystem>

int main() {

    auto alg = SpadeAlgorithm(5, 10000);

    std::string dictionary_filename = "/Volumes/SanDisk/med_vocabulary_stem.csv";
    alg.fetchDictionary(dictionary_filename);

    std::string dir = "/Volumes/SanDisk/med_data_stem/";
    for(const std::filesystem::directory_entry &dir_entry: std::filesystem::directory_iterator{dir}){
        std::string filename = dir_entry.path().string();
        std::string only_base_filename = filename.substr(filename.find_last_of("/\\") + 1);

        std::cout << only_base_filename << std::endl;

        alg.reset();
        alg.fetchData(filename, 0.03);
        alg.run();

        std::ofstream ofs("/Volumes/SanDisk/med_results_stem/"+only_base_filename+".txt");
        ofs << alg << std::endl;
        std::cout << only_base_filename << std::endl;
        std::cout << alg << std::endl;
    }


    return 0;
}
