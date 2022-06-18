#include <hashing/fnv_hash.hpp>
#include <hash_map/hash_map.hpp>
#include <hash_map/hash_map_builder.hpp>
#include <coders/bool_coder.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "utils.hpp"

void test_phf();

int main()
{
    test_phf();
}

void test_phf()
{
    auto xy = std::vector<std::pair<std::string,int>>();
    xy[0] = std::make_pair("abc",1);
    xy[1] = std::make_pair("def",2);
    xy[2] = std::make_pair("ghi",3);
    xy[3] = std::make_pair("jkl",4);
    xy[4] = std::make_pair("mno",5);

    auto start = std::chrono::system_clock::now();

    auto f = bernoulli::hash_map_builder<hashing::fnv_hash,coders::bool_decoder>().
        timeout(std::chrono::seconds(10)).debugging().
        index(0,20000000)(xy);

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    std::cout << (double)elapsed.count() / 1000 << '\n';
    std::cout << "error rate = " << f.error_rate() * 100 << "%\n";

    for (auto [x,y] : xy)
    {
        auto res = f(x);
        if (res != y)
            std::cout << "error on f(" << x << ") = " << y << "; got f(" << x << ") = " << f(x) << "\n";
    }
}
