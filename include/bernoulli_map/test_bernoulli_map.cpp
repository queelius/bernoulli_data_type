#include <hashing/fnv_hash.hpp>
#include <bernoulli_map/bernuolli_map.hpp>
#include <vector>
#include <iostream>
#include <unordered_set>
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

    auto f = bernoulli_map::bernoulli_map_builder<hashing::fnv_hash,fixed_width_int_decoder>().
        timeout(std::chrono::seconds(10)).
        index(0,20000000)(xs);

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    std::cout << (double)elapsed.count() / 1000 << '\n';
    std::cout << "error rate = " << ph.error_rate() * 100 << "%\n";

    std::unordered_set<size_t> hashes;
    for (auto x : xs)
    {
        auto h = ph(x);
        if (hashes.count(h) != 0)
            std::cout << "collision: ph(" << x << ") = " << h << "\n";
        hashes.insert(h);
    }
}
