#include "PmergeMe.hpp"
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Error: Lutfen siralanacak sayilari girin." << std::endl;
        return 1;
    }

    try {
        PmergeMe sorter;
        
        sorter.parseArguments(argc, argv);
        
        
        sorter.execute();
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}