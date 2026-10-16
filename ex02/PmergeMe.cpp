#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <sys/time.h>
#include <algorithm>

PmergeMe::PmergeMe(void) {}

PmergeMe::PmergeMe(const PmergeMe& src) : _vec(src._vec), _deq(src._deq) {}

PmergeMe::~PmergeMe(void) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) {
    if (this != &rhs) {
        _vec = rhs._vec;
        _deq = rhs._deq;
    }
    return *this;
}

void PmergeMe::parseArguments(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::stringstream ss(argv[i]);
        std::string token;

        while (ss >> token) {
            for (size_t j = 0; j < token.length(); ++j) {
                if (j == 0 && token[j] == '+') continue;
                if (token[j] < '0' || token[j] > '9')
                    throw std::runtime_error("Error: Sadece pozitif tam sayilar girilmelidir.");
            }

            long val = std::strtol(token.c_str(), NULL, 10);
            if (val <= 0 || val > 2147483647)
                throw std::runtime_error("Error: Gecersiz sayi formati.");

            if (std::find(_vec.begin(), _vec.end(), static_cast<int>(val)) != _vec.end())
                throw std::runtime_error("Error: Ayni sayidan birden fazla girilemez.");

            _vec.push_back(static_cast<int>(val));
            _deq.push_back(static_cast<int>(val));
        }
    }

    if (_vec.empty())
        throw std::runtime_error("Error: Siralanacak sayi bulunamadi.");
}

void PmergeMe::execute(void) {
    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); ++i) {
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;

    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    sortVector(_vec);
    
    gettimeofday(&end, NULL);
    double timeVec = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    
    sortDeque(_deq);
    
    gettimeofday(&end, NULL);
    double timeDeq = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    std::cout << "After:  ";
    for (size_t i = 0; i < _vec.size(); ++i) {
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << timeVec << " us" << std::endl;
              
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque  : " << timeDeq << " us" << std::endl;
}

void PmergeMe::sortVector(std::vector<int>& vec) {
    if (vec.size() <= 1)
        return;

    int oddNumber = -1;
    bool odd = false;

    if (vec.size() % 2 != 0) {
        oddNumber = vec.back();
        vec.pop_back();
        odd = true;
    }

    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < vec.size(); i += 2) {
        if (vec[i] > vec[i + 1])
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
        else
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
    }

    std::vector<int> mainChain;
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].second);
    }

    sortVector(mainChain);

    std::vector<int> pendChain;
    for (size_t i = 0; i < mainChain.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].second == mainChain[i]) {
                pendChain.push_back(pairs[j].first);
                break;
            }
        }
    }

    if (!pendChain.empty()) {
        mainChain.insert(mainChain.begin(), pendChain[0]);
    }

    std::vector<int> jacob = generateJacobsthal(pendChain.size());
    size_t insertedCount = 1; 

    for (size_t i = 1; i < jacob.size(); ++i) {
        int currentJacob = jacob[i];
        int prevJacob = jacob[i - 1];

        for (int j = currentJacob; j > prevJacob; --j) {
            if (j - 1 > 0 && static_cast<size_t>(j - 1) < pendChain.size()) {
                int valToInsert = pendChain[j - 1];
                
                std::vector<int>::iterator it = std::lower_bound(
                    mainChain.begin(), 
                    mainChain.end(), 
                    valToInsert
                );
                
                mainChain.insert(it, valToInsert);
                insertedCount++;
            }
        }
    }

    if (odd) {
        std::vector<int>::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), oddNumber);
        mainChain.insert(it, oddNumber);
    }

    vec = mainChain;
}

void PmergeMe::sortDeque(std::deque<int>& deq) {
    if (deq.size() <= 1)
        return;

    int oddNumber = -1;
    bool odd = false;

    if (deq.size() % 2 != 0) {
        oddNumber = deq.back();
        deq.pop_back();
        odd = true;
    }

    std::deque<std::pair<int, int> > pairs;
    for (size_t i = 0; i < deq.size(); i += 2) {
        if (deq[i] > deq[i + 1])
            pairs.push_back(std::make_pair(deq[i + 1], deq[i]));
        else
            pairs.push_back(std::make_pair(deq[i], deq[i + 1]));
    }

    std::deque<int> mainChain;
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].second);
    }

    sortDeque(mainChain);

    std::deque<int> pendChain;
    for (size_t i = 0; i < mainChain.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].second == mainChain[i]) {
                pendChain.push_back(pairs[j].first);
                break;
            }
        }
    }

    if (!pendChain.empty()) {
        mainChain.push_front(pendChain[0]);
    }

    std::vector<int> jacob = generateJacobsthal(pendChain.size());
    size_t insertedCount = 1;

    for (size_t i = 1; i < jacob.size(); ++i) {
        int currentJacob = jacob[i];
        int prevJacob = jacob[i - 1];

        for (int j = currentJacob; j > prevJacob; --j) {
            if (j - 1 > 0 && static_cast<size_t>(j - 1) < pendChain.size()) {
                int valToInsert = pendChain[j - 1];
                
                std::deque<int>::iterator it = std::lower_bound(
                    mainChain.begin(), 
                    mainChain.end(), 
                    valToInsert
                );
                
                mainChain.insert(it, valToInsert);
                insertedCount++;
            }
        }
    }

    if (odd) {
        std::deque<int>::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), oddNumber);
        mainChain.insert(it, oddNumber);
    }

    deq = mainChain;
}

std::vector<int> PmergeMe::generateJacobsthal(int size) {
    std::vector<int> jacob;
    
    if (size == 0) return jacob;
    jacob.push_back(0);
    
    if (size == 1) return jacob;
    jacob.push_back(1);

    int i = 2;
    while (true) {
        int nextJacob = jacob[i - 1] + 2 * jacob[i - 2]; 
        jacob.push_back(nextJacob);
        
        if (nextJacob >= size)              
            break;
        i++;
    }
    
    return jacob;
}