#include <iostream>
#include "space_optimal_bloom_filter.hpp"

int main()
{
    size_t N = 1000;
    size_t S = 100000;
    std::vector<std::string> xs;
    for (int x = 0; x < N; ++x)
    {
        xs.push_back(std::to_string(x));
    }
    double true_fpr_spec = .01;
    auto bf = space_optimal_bloom_filter<std::string>(xs.begin(),xs.end(),true_fpr_spec);
    auto bf1 = space_optimal_bloom_filter<std::string>(xs.begin(),xs.begin()+100,true_fpr_spec);
    auto bf2 = space_optimal_bloom_filter<std::string>(xs.begin(),xs.begin()+200,true_fpr_spec);
    auto bf3 = space_optimal_bloom_filter<std::string>(xs.begin()+10,xs.begin()+130,true_fpr_spec);
    auto bf4 = space_optimal_bloom_filter<std::string>(xs.begin()+20,xs.begin()+140,true_fpr_spec);
    auto bf5 = space_optimal_bloom_filter<std::string>(xs.begin()+30,xs.begin()+150,true_fpr_spec);
    auto bf6 = space_optimal_bloom_filter<std::string>(xs.begin()+300,xs.begin()+600,true_fpr_spec);
    auto bf7 = space_optimal_bloom_filter<std::string>(xs.begin()+230,xs.begin()+450,true_fpr_spec);
    auto bf8 = space_optimal_bloom_filter<std::string>(xs.begin()+130,xs.begin()+350,true_fpr_spec);

    std::cout << "k=" << bf.k() << "\n";
    std::cout << "m=" << bf.m() << "\n";
    std::cout << "n=" << bf.n() << "\n";
    std::cout << "fpr=" << bf.fpr() << "\n";
    std::cout << "hash(bf)=" << std::hash<space_optimal_bloom_filter<std::string>>{}(bf) << "\n";

    int count = 0;
    for (int x = N; x < S + N; ++x)
    {
        if (bf(std::to_string(x)))
        {
            ++count;
            //std::cout << "fp on " << std::to_string(x) << "\n";
        }
    }
    std::cout << "sampled fpr=" << count / (double)S << "\n";

    std::vector<space_optimal_bloom_filter<std::string>> bfs;
    bfs.push_back(bf);
    bfs.push_back(bf1);
    bfs.push_back(bf2);
    bfs.push_back(bf3);
    bfs.push_back(bf4);
    bfs.push_back(bf5);
    bfs.push_back(bf6);
    bfs.push_back(bf7);
    bfs.push_back(bf8);
    auto bff = space_optimal_bloom_filter<space_optimal_bloom_filter<std::string>>(bfs.begin(),bfs.end(),.2);

    std::cout << "k=" << bff.k() << "\n";
    std::cout << "m=" << bff.m() << "\n";
    std::cout << "n=" << bff.n() << "\n";
    std::cout << "fpr=" << bff.fpr() << "\n";
    //std::cout << "hash(bff)=" << std::hash<space_optimal_bloom_filter<space_optimal_bloom_filter<std::string>>>{}(bff) << "\n";

    for (auto bf : bfs)
    {
        std::cout << bff(bf) << "\n";
    }

    int count2 = 0;
    double fpr = 0.0001;
    for (int x = 0; x < 1000; ++x)
    {
        auto bf = space_optimal_bloom_filter<std::string>(xs.begin(),xs.end(),fpr);
        fpr += 0.001;

        if (bff(bf))
            ++count2;
    }
    std::cout << "sampled fpr2=" << count2 / 1000.0 << "\n";

}