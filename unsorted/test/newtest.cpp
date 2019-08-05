#include <iostream>
#include <tuple>
#include "tl.hpp"

struct param_set
{
    param_set(int n, double p) : n(n), p(p) {};

    //using types = typename std::tuple<int, double>;
    using types = tl<int,double>;
    
    constexpr int size() const { return types::size(); };

    int n;
    double p;
};


int main()
{
    param_set x(10, .5);
    std::cout << x.size() << std::endl;

    if constexpr (x.size() == 1)
    {
        type_list_extract_t<0, param_set::types> x1;
    }
    if constexpr  (x.size() == 2)
    {
        type_list_extract_t<0, param_set::types> x1;
        type_list_extract_t<1, param_set::types> x2;
    }
    if constexpr (x.size() == 3)
    {
        type_list_extract_t<0, param_set::types> x1;
        type_list_extract_t<1, param_set::types> x2;
        type_list_extract_t<2, param_set::types> x3;
    }


    //std::get<0>(x.values);
}