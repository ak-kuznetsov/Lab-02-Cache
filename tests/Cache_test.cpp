// Copyright 2020 ak-kuznetsov <a.kuznetsov2701@mail.ru>

#include <Cache.hpp>

#include <gtest/gtest.h>

TEST(Cache, EmptyTest) {
    std::vector<Type> types {Straight, Reverse, Random};
    Cache cacher{types};
    std::cout << cacher;
}