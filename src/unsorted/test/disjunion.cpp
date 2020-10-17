#include <iostream>
#include <type_traits>
#include <variant>
#include <tuple>
#include "typelist.hpp"
#include <string>

template <typename T>
struct Set
{
    using value_type = T;

    bool contains(T x) const { return true; };
};

template <typename L, typename R>
struct DisjointUnion
{
    using sum_type = sum_variant<
        typename L::value_type,
        typename R::value_type>;

    using value_type_left = typename sum_type::left_type;
    using value_type_right = typename sum_type::right_type;
    using value_type = typename sum_type::type;

    using type_list_left = typename sum_type::type_list_left;
    using type_list_right = typename sum_type::type_list_right;
    using type_list = typename sum_type::type_list;

    DisjointUnion(L l, R r) : l(l), r(r) {};

    bool contains(value_type x) const
    {
        
        // x is a type in variant<...> with both left and right types
        // we want to see if x is a type that is in left's typelist.
        //if (std::holds_alternative<value_type_left>(x))
        {
            //std::cout << "left\n";
            //return l.contains(x);
        }
        //std::cout << "right\n";
        //value_type_right rx = x;
        //return r.contains(x);
        return true;
    };

    L l;
    R r;
};




int main()
{
    using A = Set<std::variant<int,char>>;
    using B = Set<bool>;
    using C = Set<std::string>;
    using D = Set<unsigned int>;

    A a;
    B b;
    C c;
    D d;

    DisjointUnion<A,B> ab(a,b);
    ab.contains(2);

    std::cout << std::is_same<std::variant<int,char,bool>,DisjointUnion<A,B>::value_type>::value << std::endl;
    std::cout << 
        std::is_same<
            std::variant<int,char,bool,std::string,unsigned int>,
            DisjointUnion<DisjointUnion<A,B>,DisjointUnion<C,D>>::value_type>::value
        << std::endl;
}