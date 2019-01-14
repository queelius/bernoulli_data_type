#include <iostream>
#include "interval.hpp"

template <typename T, class OperatorSet>
std::ostream& operator<<(std::ostream& out, alex::interval<T, OperatorSet> x)
{
    out << "[" << x.min() << "," << x.max() << "]";
    return out;
};

int main()
{
    using interval0_1 = alex::interval<
        double, alex::bounded_arithmetic_operator_set<alex::LB_0, alex::UB_1>>;

    interval0_1 x{0.5, 2.0};
    interval0_1 y{.2,5};

    auto z = x + y;
    std::cout << z << std::endl;
}