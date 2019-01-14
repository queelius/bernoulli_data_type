#include <iostream>
#include "interval.hpp"

template <typename T>
std::ostream& operator<<(std::ostream& out, alex::interval<T> x)
{
    out << '[' << x.min() << ',' << x.max() << ']';
    return out;
};

int main()
{
    auto x1 = alex::make_interval(0, 1);
    auto x2 = alex::make_interval(.5, 1.1);
    auto x3 = 3.5;
    auto x4 = alex::make_interval(-1, 0.5);

    {
        using namespace alex::min_span_set_theory_op_set;
        std::cout << x1 << '+' << x2 << '+' << x3 << '=' << (x1 + x2 + x3) << std::endl;
        std::cout << '(' << x1 << '+' << x2 << '+' << x3 << ')' << '*' << x4 << '=' << (x1 + x2 + x3) * x4 << std::endl;
    }

    {
        using namespace alex::interval_arithmetic_op_set;
        std::cout << x1 << '+' << x2 << '=' << (x1 + x2 + x4) << std::endl;
        //std::cout << x1 << '+' << x2 << '+' << x3 << '=' << (x1 + x2 + x3) << std::endl;
        //std::cout << '(' << x1 << '+' << x2 << '+' << x3 << ')' << '*' << x4 << '=' << (x1 + x2 + x3) * x4 << std::endl;
    }
}