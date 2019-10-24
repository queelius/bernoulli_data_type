#include <iostream>
#include <set/iterable/empty_set.hpp>
#include <set/iterable/set_container.hpp>
#include <set/iterable/power_set.hpp>
#include <set/iterable/cartesian_product.hpp>
#include <set/iterable/empty_set.hpp>
#include <set/iterable/universal_set.hpp>
#include <set>


template <typename A, typename B>
std::ostream& operator<<(std::ostream& out, std::pair<A,B> p)
{
    out << "(" << p.first << "," << p.second << ")";
    return out;
}


template <typename T>
std::ostream& operator<<(std::ostream& out, std::set<T> s)
{
    out << "{";
    bool first = true;
    for (auto x : s)
    {
        if (!first)
        {
            out << ",";
        }
        out << x;
        first = false;
    }
    out << "}\n";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, alex::set::iterable::Set<T> s)
{
    out << "{";
    bool first = true;
    for (auto x : s)
    {
        if (!first)
        {
            out << ",";
        }
        out << x;
        first = false;
    }
    out << "}\n";
    return out;
}

template <typename S>
std::ostream& operator<<(std::ostream& out, alex::set::iterable::UniversalSet<S> s)
{
    out << "{";
    bool first = true;
    for (auto x : s)
    {
        if (!first)
        {
            out << ",";
        }
        out << x;
        first = false;
    }
    out << "}\n";
    return out;
}


template <typename T, typename I, template <typename> typename C>
std::ostream& operator<<(std::ostream& out, alex::set::iterable::PowerSet<T,I,C> s)
{
    out << "{";
    bool first = true;
    for (auto const & x : s)
    {
        if (!first)
        {
            out << ",";
        }
        out << x;
        first = false;
    }
    out << "}\n";
    return out;
}

template <typename A, typename B, typename I>
std::ostream& operator<<(std::ostream& out, alex::set::iterable::CartesianProduct<A,B,I> s)
{
    out << "{";
    bool first = true;
    for (auto const & x : s)
    {
        if (!first)
        {
            out << ",";
        }
        out << x;
        first = false;
    }
    out << "}\n";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, alex::set::iterable::EmptySet<T> s)
{
    out << "{}";
    return out;
}


int main()
{
    namespace a = alex::set::iterable;

    auto u = a::make_universal_set(alex::set::iterable::Set<int>{1,2});
    auto s = a::Set<char>{};
    auto b = a::Set<int>{1,2};
    auto pu = a::make_power_set(u);

    auto rel3 = a::make_cartesian_product(b,s);
    for (auto const & x : pu)
    {
        std::cout << x << std::endl;
    }

    /*
    alex::set::iterable::Set<int> s(std::set<int>{1,2});
    alex::set::iterable::Set<char> t(std::set<char>{'a','b'});
    auto car = alex::set::iterable::make_cartesian_product(s, t);
    auto total_relation = alex::set::iterable::make_power_set(car);

    std::cout << total_relation.cardinality() << std::endl;
    std::cin.get();

    std::cout << total_relation << std::endl;
    */
}