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

const int Size[3]{256, 1024, 8192};
const int Research{3};
const int Count{1000};
const int byte_in_kb{1024};
const int byte_in_int{4};
const int items_in_line{16};

enum Type {
    Straight, Reverse, Random
};

class Cache {
private:
    int Num_Exp = 0;
    std::vector<std::vector<int>> Duration;
    std::vector<int> Exp_Size;
    std::vector<std::string> Buffer_Size;
    std::string Exp_Name[Research];

public:
    explicit Cache(std::vector<Type>);

    void Find_Exp_Size();

    friend std::ostream& operator<<(std::ostream&, const Cache&);
};
#endif // INCLUDE_CACHE_HPP_