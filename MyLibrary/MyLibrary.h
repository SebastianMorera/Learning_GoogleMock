#ifndef TEACHINGPROJECT3_LIBRARY_H
#define TEACHINGPROJECT3_LIBRARY_H

#include <iostream>
#include <vector>

using namespace std;

// Basic mock
class SomeClass
{
    public:
        SomeClass() = default;
        virtual void someMethod();

};

// Assertions on vectors
std::vector<int> generateNumbers(int n, int limit);

#endif //TEACHINGPROJECT3_LIBRARY_H