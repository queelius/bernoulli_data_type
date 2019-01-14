#include <iostream>
#include <utility>
#include <vector>
#include "interval0_1.hpp"
#include "union_aset.hpp"
#include "disjoint_interval_set.hpp"

template <typename ValueType,
    typename FalsePositiveRateType = double,
    typename FalseNegativeRateType = double>
class aset
{
public:
    using value_type = ValueType;
    using fpr_type = FalsePositiveRateType;
    using fnr_type = FalseNegativeRateType;

    aset(FalsePositiveRateType fpr, FalseNegativeRateType fnr) :
        _fpr(fpr), _fnr(fnr) {};

    FalsePositiveRateType fpr() const { return _fpr; };
    FalseNegativeRateType fnr() const { return _fnr; };

    bool contains(const ValueType&) { return true; };

private:
    FalsePositiveRateType _fpr;
    FalseNegativeRateType _fnr;
};

template <typename T>
std::ostream& operator<<(std::ostream& outs, const alex::math::interval0_1<T>& x)
{
    outs << "[" << x.min() << "," << x.max() << "]";
    return outs;
}

template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<alex::math::interval0_1<T>>& x)
{
    for (const auto& i : x)
        outs << i << ",";
    return outs;
}

using namespace alex::aset;

int main()
{
    //alex::math::interval0_1<double> a1 = alex::math::make_interval0_1(0,1);
    //alex::math::interval0_1<double> a2 = alex::math::make_interval0_1(0,1);
    //std::cout << (1 - a1 - a2) << std::endl;
    //exit(0);
    auto fpr1 = alex::math::make_interval0_1(.1, .2);
    auto fnr1 = alex::math::make_interval0_1(.075, .15);
    auto fpr2 = alex::math::make_interval0_1(.025, .05);
    auto fnr2 = alex::math::make_interval0_1(.125, .13);

    auto fpr = fpr1 + fpr2 - fpr1 * fpr2;

    auto x1 = fnr1.min() * (1. - fpr2.max());
    auto x2 = fnr2.min() * (1. - fpr1.max());
    auto x3 = fnr1.min() * fnr2.min();
    auto min_fnr = std::min(std::min(x1, x2), x3);

    auto y1 = fnr1.max() * (1. - fpr2.min());
    auto y2 = fnr2.max() * (1. - fpr1.min());
    auto y3 = fnr1.max() * fnr2.max();
    auto max_fnr = std::max(std::max(y1, y2), y3);

    auto i1 = fnr1 * (1 - fpr2);
    auto i2 = fnr2 * (1 - fpr1);
    auto i3 = fnr1 * fnr2;

    std::cout << "fnr1(1-fpr2) -> " << i1 << std::endl;
    std::cout << "fnr2(1-fpr1) -> " << i2 << std::endl;
    std::cout << "fnr1*fnr2 -> " << i3 << std::endl;

    std::cout << "fnr -> min(" << x1 << ", " << x2 << ", " << x3 << ") = " << min_fnr << std::endl;
    std::cout << "fnr -> max(" << y1 << ", " << y2 << ", " << y3 << ") = " << max_fnr << std::endl;

    // auto disj_ints = alex::math::make_disjoint_interval_set(std::vector<alex::math::interval0_1<double>>({i1,i2,i3}));
    // std::cout << "disjoint intervals: ";
    // for (auto x : disj_ints)
    // {
    //     std::cout << x << "; ";
    // }
    // std::cout << std::endl;

    alex::math::disjoint_interval_set0_1<double> disj;
    disj = disj.unite(i1).unite(i2).unite(i3);
    std::cout << "disjoint interval set: ";
    for (auto x : disj)
    {
        std::cout << x << "; ";
    }
    std::cout << std::endl;

    //std::cout << "is_ranged<i1 type>::value = " << alex::math::is_ranged<decltype(i1)>::value << std::endl;
    //std::cout << "is_ranged<double>::value = " << alex::math::is_ranged<double>::value << std::endl;

    auto a = aset<int, alex::math::interval0_1<double>, alex::math::interval0_1<double>>(fpr1, fnr1);
    auto b = aset<int, alex::math::interval0_1<double>, alex::math::interval0_1<double>>(fpr2, fnr2);
    auto ab = union_of(a, b);

    std::cout << "a.fpr() -> " << a.fpr() << std::endl;
    std::cout << "a.fnr() -> " << a.fnr() << std::endl;

    std::cout << "b.fpr() -> " << b.fpr() << std::endl;
    std::cout << "b.fnr() -> " << b.fnr() << std::endl;

    std::cout << "ab.fpr() -> " << ab.fpr() << std::endl;
    std::cout << "ab.fnr() -> " << ab.fnr() << std::endl;

    std::cout << "ab.contains(...) = " << ab.contains(1.2) << std::endl;

    //auto r = union_of(ab, a);
    //auto fpr_r = r.fpr();
    //auto fnr_r = r.fnr();
}