#include <iostream>
#include <utility>
#include <vector>
#include "interval.hpp"
#include "disjoint_interval_set.hpp"

template <class A, class B,
    typename = typename std::enable_if<std::is_same<typename A::value_type, typename B::value_type>::value>::type>
class union_aset
{
public:
    using value_type = typename A::value_type;
    using fpr_type_A = decltype(std::declval<A>().fpr());
    using fnr_type_A = decltype(std::declval<A>().fnr());
    using fpr_type_B = decltype(std::declval<B>().fpr());
    using fnr_type_B = decltype(std::declval<B>().fnr());


    union_aset(A& a, B& b) : a(a), b(b) {};

    bool contains(const value_type& x) { return a.contains(x) || b.contains(x); };

    fpr_type_A fpr() const
    {
        return 1 - (1 - a.fpr()) * (1 - b.fpr());
    };

    fnr_type_A fnr() const
    {
        return a.fnr();
    };


private:
    A a;
    B b;
};

template <typename T>
class set1
{
public:
    using value_type = T;

    double fpr() const { return .1; };
    double fnr() const { return .05; };

    bool contains(T) { return true; };
};

template <typename T>
class set2
{
public:
    using value_type = int;

    double fpr() const { return .01; };
    double fnr() const { return .15; };

    bool contains(T) { return false; };
};

template <class A, class B>
auto union_of(A a, B b)
{
    return union_aset<A, B>(a, b);
};

int main()
{
    std::vector<alex::math::interval<double>> intervals1(
    {
        alex::math::make_interval(0, 1.),
        alex::math::make_interval(2., 3.)
    });

    std::vector<alex::math::interval<double>> intervals2(
    {
        alex::math::make_interval(-.5, .5),
        alex::math::make_interval(2.5, 3.),
        alex::math::make_interval(3.5, 4.),
    });

    auto united = alex::math::unite_disjoint_interval_sets(intervals1, intervals2);
    std::cout << "united\n";
    for (auto x : united)
    {
        std::cout << x.min() << ", " << x.max() << std::endl;
    }

    std::cout << "complemented\n";
    auto comp = alex::math::complement_disjoint_interval_set(united);
    for (auto x : comp)
    {
        std::cout << x.min() << ", " << x.max() << std::endl;
    }
}