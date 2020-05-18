#include <iostream>
#include "Vector.h"
#include <vector>
#include <chrono>

void runVectorComparisonTests();

int main() {
    runVectorComparisonTests();

    return 0;
}

void runVectorComparisonTests() {
    std::vector<unsigned int> szs = { 10000, 100000, 1000000, 10000000, 100000000 };

    std::chrono::duration<double> diff;

    for (auto sz = szs.begin(); sz != szs.end(); ++sz) {
        auto t1 = std::chrono::high_resolution_clock::now();
        std::vector<int> v1;
        v1.reserve(*sz);
        for (int i = 1; i <= *sz; ++i)
            v1.push_back(i);
        auto t2 = std::chrono::high_resolution_clock::now();
        diff = t2 - t1;
        std::cout << "Laikas " << *sz << " elementu uzpildymui std::vector: " << diff.count() << "s\n";
        v1.clear();


        t1 = std::chrono::high_resolution_clock::now();
        Vector<int> v2;
        v2.reserve(*sz);
        for (int i = 1; i <= *sz; ++i)
            v2.push_back(i);
        t2 = std::chrono::high_resolution_clock::now();
        diff = t2 - t1;
        std::cout << "Laikas " << *sz << " elementu uzpildymui Vector: " << diff.count() << "s\n";
        v2.clear();
    }
}
