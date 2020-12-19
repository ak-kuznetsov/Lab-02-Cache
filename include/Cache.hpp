// Copyright 2020 ak-kuznetsov <a.kuznetsov2701@mail.ru>

#ifndef INCLUDE_CACHE_HPP_
#define INCLUDE_CACHE_HPP_

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>

const int Cache_Size[3]{256, 1024, 8192};
const int Num_Research{3};
const int Test_Count{1000};
const int Num_byte_In_Long_double{16};
const int Num_b_In_kb{1024};
const int Num_Data{4};

enum Type {
    Straight, Reverse, Random
};

class Cache {
public:
    explicit Cache(std::vector<Type>);

    void Find_Exp_Size();

    friend std::ostream& operator<<(std::ostream&, const Cache&);

private:
    int Num_Experiment = 0;
    std::vector<std::vector<int>> Duration;
    std::vector<int> Experiment_Size;
    std::vector<std::string> Experiment_Size_Name;
    std::string Experiment_Type_Name[Num_Research];
};
#endif // INCLUDE_CACHE_HPP_