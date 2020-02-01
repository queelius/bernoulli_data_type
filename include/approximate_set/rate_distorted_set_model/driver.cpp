#include "approximate_set.hpp"
#include "random_approximate_set.hpp"
#include "interval.hpp"
using namespace alex::approximate_set;

#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>

int main()
{
    auto a1 = random_approximate_set<int>({1,2,3}, .1, .85);
    auto a2 = random_approximate_set<int>({3,4,5}, .2, .9);

    int N = 5;

    for (int i = 0; i <= N; ++i)
        std::cout << a1.contains(i) << "\n";

    std::cout << "\nwrapper\n";
    auto w1 = make_wrapper(a1);
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << w1.contains(i) << "\n";
    std::cout << "\n";

    std::cout << "\ncomplement\n";
    auto c1 = make_complement(a1);
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << c1.contains(i) << "\n";

    std::cout << "\nempty\n";
    auto e1 = make_empty<int>();
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << e1.contains(i) << "\n";

    std::cout << "\nuniversal\n";
    w1 = make_universal<int>();
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << w1.contains(i) << "\n";

    std::cout << "\nunion\n";
    auto w2 = make_union(a1,a2);
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << w2.contains(i) << "\n";

    std::cout << "\nunion2\n";
    auto w3 = make_union(w2,a2);
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << w3.contains(i) << "\n";

    std::cout << "\nintersection\n";
    auto i1 = make_intersection(a1,a2);
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << i1.contains(i) << "\n";


    std::cout << "\nintersection2\n";
    i1 = make_complement(make_union(make_complement(a1),make_complement(a2)));
    for (int i = 0; i <= N; ++i)
        std::cout << i << " : " << i1.contains(i) << "\n";


    std::cout << "\n";

    //auto u = make_union(a1, a2);
}