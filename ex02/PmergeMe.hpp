#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <exception>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int>  _deq;

    void sortVector(std::vector<int>& vec);
    void sortDeque(std::deque<int>& deq);

    std::vector<int> generateJacobsthal(int size);

public:
    PmergeMe(void);
    PmergeMe(const PmergeMe& src);
    ~PmergeMe(void);
    PmergeMe& operator=(const PmergeMe& rhs);


    void parseArguments(int argc, char** argv);

    void execute(void);
};

#endif