#include <random>
#include <utility>
#include <algorithm>
#include <set>
#include <map>
#include <variant>


template <typename T>
struct EmptySet
{
    bool contains(T const &) const { return false; };
};

template <typename T>
struct UniversalSet
{
    bool contains(T const &) const { return true; };
};


template <
    typename T,
    template <typename> typename L,
    template <typename> typename R
>
auto make_union(L<T> left, R<T> right)
{
    return Union<T,L,R>(left, right);
}

template <
    typename T,
    typename R
>
auto
make_union(EmptySet<T> left, R right)
{
    return right;
};

template <
    typename L, // L models Approximate Set concept
    typename T 
>
auto
make_union(L left, EmptySet<T> right)
{
    return left;
};

template <
    typename L, // L models Approximate Set concept
    typename T 
>
auto
make_union(L left, UniversalSet<T> right)
{
    return right;
};

template <
    typename L, // L models Approximate Set concept
    typename T 
>
auto
make_union(UniversalSet<T> left, R right)
{
    return left;
};

template <
    typename L,
    typename R
>
auto
make_disjoint_union(L left, R right)
{
    return DisjointUnion<L,R>(left,right);
}

template <
    typename T,
    typename R
>
auto
make_disjoint_union(EmptySet<T> left, R right)
{
    return right;
};

template <
    typename Left,
    typename T
>
auto
make_disjoint_union(Left left, EmptySet<T> right)
{
    return left;
};

template <
    typename T,
    typename Right
>
auto
make_intersection(EmptySet<T> left, Right right)
{
    return left;
}

template <
    typename T,
    typename Right
>
auto
make_intersection(EmptySet<T> left, Right right)
{
    return left;
};

template <
    typename T,
    typename Right
>
auto
make_cartesian_product(EmptySet<T> left, Right right)
{
    return left;
};

template <
    typename Left,
    typename T
>
auto
make_cartesian_product(Left left, EmptySet<T> right)
{
    return right;
};

template <
    typename Left,
    typename Right
>
auto
make_cartesian_product(Left left, Right right)
{
    return CartesianProduct<Left,Right>(left,right);
};

};