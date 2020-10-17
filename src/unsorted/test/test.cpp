#include <iostream>
#include <type_traits>
#include <variant>
#include "tl.hpp"

template <typename T>
struct Set
{
    using value_type = T;
    bool contains(T x) const
    {
        std::cout << label << "\n";
        return true;
    };

    std::string label;
};


template <std::variant<As...>, std::variant<Bs...>>
struct disjoint_union
{

}



template <typename L, typename R>
struct DisjointUnionSet
{
    using type_list_left = decltype(as_type_list((typename L::value_type){}));
    using type_list_right = decltype(as_type_list((typename R::value_type){}));
    using type_list = append_t<type_list_left,type_list_right>;
    using value_type = decltype(as_variant(type_list{}));
    using value_type_left = decltype(as_variant(type_list_left{}));
    using value_type_right = decltype(as_variant(type_list_right{}));

    DisjointUnionSet(L l, R r) : l(l), r(r)
    {

    };

    struct sum_type
    {
        bool operator()(int x)
        {
            return true;
        };

        bool operator()(char x)
        {
            return true;
        };

        bool operator()(bool x)
        {
            return true;
        };

        bool operator()(double x)
        {
            return true;
        };
    };    

    bool contains(value_type x) const
    {
        auto x0 = std::get_if<value_type_left>(&x);
        if (x0)
        {
            if (l.contains(*x0)) return true;
        }
        auto x1 = std::get_if<value_type_right>(&x);
        if (x1)
        {
            if (r.contains(*x1)) return true;
        }
        return false;

        //return l.contains(std::get<0>(x)) || r.contains(std::get<1>(x));
        //switch (x.index())
        //{
        //    case 0: return l.contains(std::get<0>(x));
         //   default: return r.contains(std::get<1>(x));
       // }
        //if (std::holds_alternative<value_type_left>(x))
        //    return l.contains(std::get<value_type_left>(x));
        //else
        //    return r.contains(std::get<value_type_right>(x));
    };

    L l;
    R r;
};

template <
        typename L, // L models an approximate_set<ValueType(L)>
        typename R> // R models an approximate_set<ValueType(R)>
auto disjoint_union(L l, R r)
{
    return DisjointUnionSet<L,R>(std::move(l), std::move(r));
};

int main()
{
    Set<int> a{"a"};
    Set<char> b{"b"};
    Set<bool> c{"c"};
    Set<double> d{"d"};
    auto ab = disjoint_union(a,b);
    auto abc = disjoint_union(ab,c);
    auto abcd = disjoint_union(abc,d);

    abcd.contains(1);
    //DisjointUnionSet<Set<int>,Set<char>> ab = {a,b};

    //ab.contains(1);
    //abc.contains(1);
}

    /*
    using T1 = std::variant<int,bool>;
    using T2 = std::variant<double,char>;
    using T3 = std::variant<T1,T2>;

    T3 x = 3.2;

    struct visitor
    {
        void operator()(T1 x)
        {
            std::cout << "a\n";
        };
        void operator()(T2 x)
        {
            std::cout << "b\n";
        };
        //void operator()(T3 x)
       // {
       //     std::visit(visitor, x);
       // };

    }

    std::visit(visitor{}, x);
    //std::visit([](auto&& x) { std::visit([](auto&& x) { std::cout << x << std::endl;}, x); }, x);

    //bool b = true;
    //auto bb = ::max(a,b);
    //std::cout << bb << std::endl;

    //std::cout << test() << std::endl;
    //std::cout << test(2) << std::endl;
    //param_set test;

    test();


/*
template<typename T1, typename T2>
std::common_type_t<T1,T2> max (T1 a, T2 b)
{
    return a;
}

template <class H, class T>
struct type_list
{
    typedef H head;
    typedef T tail;
};

struct null_type_list {};

// params provides a linearization
// to the parameter space, i.e.,
// parameter set objects are tuple-like.
template <typename P>
struct param_set
{
    param_set(int n, P p) : n(n), p(p) {};

    constexpr int size() const { return 2; };

    int n;
    P p;
};
*/




/*
template <typename L, typename R>
struct DisjointUnionSet2
{
    using type_list_left = decltype(as_type_list((typename L::value_type){}));
    using type_list_right = decltype(as_type_list((typename R::value_type){}));
    using type_list = append_t<type_list_left,type_list_right>;
    using value_type = decltype(as_variant(type_list{}));
    using value_type_left = decltype(as_variant(type_list_left{}));
    using value_type_right = decltype(as_variant(type_list_right{}));

    //using type_list_left = decltype(as_type_list((typename L::value_type){}));
    //using type_list_right = decltype(as_type_list((typename R::value_type){}));
    //using type_list = append_t<type_list_left,type_list_right>;
    //using value_type = decltype(as_variant(type_list{}));


    //using value_type_left = typename L::value_type;
    //using value_type_right = typename R::value_type;
    //using value_type = std::variant<value_type_left, value_type_right>;

    DisjointUnionSet2(L l, R r) : l(l), r(r) 
    {

    };

    bool contains(value_type x) const
    {
        if (std::holds_alternative<value_type_left>(x))
            return l.contains(std::get<value_type_left>(x));
        else
            return r.contains(std::get<value_type_right>(x));
    };

    L l;
    R r;
};
*/










    /*
    using value_type_left = std::variant<int,char>;
    using value_type_right = bool;

    using type_list_left = decltype(as_type_list((value_type_left){}));
    using type_list_right = decltype(as_type_list((value_type_right){}));
    using type_list = append_t<type_list_left,type_list_right>;
    using value_type = decltype(as_variant(type_list{}));
    using value_type_left = decltype(as_variant(type_list_left{}));
    using value_type_right = decltype(as_variant(type_list_right{}));
    */
