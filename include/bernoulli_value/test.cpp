#include <iostream>
#include <set>

/*
template <int N, typename T, typename P>
struct bernoulli {};

template <int N, typename T>
struct bernoulli<N,T,std::equal_to<T>>
{
    using bernoulli_value_type = T;
    using bernoulli_pred = std::equal_to<T>;
    static auto bernoulli_order() { return N; }
};

template <int N, typename T>
struct bernoulli<1,T,std::equal_to<T>>
{

}


template <int N, typename T>
using bernoulli_regular = bernoulli<N,T,std::equal_to<T>>;

template <typename T, typename P>
using first_order_bernoulli = bernoulli<1,T,P>;

template <typename T>
using first_order_bernoulli_regular = bernoulli_value<1,T>;

*/


template <typename T, int N, typename partition_tag>
struct bernoulli_value
{
    T value;
    double err[N];

    using bernoulli_pred_type = std::equal_to<T>;
    using value_type = T;
    auto order() const { return N; }
};

// Suppose we have a function of type X -> Y and we wish to provide as input a
// Bernoulli value of type X instead. We provide an overloaded set of functors
// fmap that automatically lifts to Bernoulli types,
//     fmap : X -> Y -> bernoulli<X,N,P> -> bernoulli<Y,M,P2>
// where in general N != M and P != P2. Since the function's mapping is
// not generally specified at the type level, we do not generally know M
// and P2. The generic fmap is then denoted by
//     fmap : X -> Y -> bernoulli_value<X,N,P> -> bernoulli<Y,-1,unknown<-1>>
// where -1 order means unknown order and unknown partition
//
// If we know more about the type then more information about the lifted type
// can be inferred, e.g.,
//     fmap : injective(X->Y) -> bernoulli<X,N,P> -> bernoulli<Y,N,unknown<N>>
// where the order is the same but the partition is generally unknown.




template <typename T>
struct bernoulli_value<T,1,struct singleton>
{
    T value;
    double err;

    using bernoulli_pred_type = std::equal_to<T>;
    using value_type = T;
    auto order() const { return 1; }
};

template <typename T>
struct bernoulli_value<T,0,struct singleton>
{
    T value;

    using bernoulli_pred_type = std::equal_to<T>;
    using value_type = T;
    auto order() const { return 0; }
};

struct bernoulli_value<bool,2,struct pos_neg>
{
    T value;
    double fpr,fnr;

    using bernoulli_pred_type = std::equal_to<T>;
    using value_type = T;
    auto order() const { return 2; }
};

template <typename T>
using bernoulli_value0 = bernoulli_value<T,0,struct singleton>;

template <typename T>
using bernoulli_value1 = bernoulli_value<T,1,struct singleton>;

/**
 * The positive-negative Bernoulli Boolean 
 * 
 * The Boolean type has two partitions, {{false,true}} and {{false},{true}}.
 * The first partition is the first-order Bernoulli Boolean and the second
 * partition is the second-order Bernoulli Boolean. No other partition is
 * possible, thus Bernoulli types isomorphic to {false,true} have a maximum
 * order of 2.
 */
template <typename T>
using bernoulli_bool1 = bernoulli_value1<bool>;


template <typename T>
using pos_neg_bernoulli_bool = bernoulli_value2<bool,struct pos_neg>;

auto fpr(bernoulli_bool1 x) { return x.err; }
auto fnr(bernoulli_bool1 x) { return x.err; }

auto fpr(pos_neg_bernoulli_bool x) { return x.fpr; }
auto fnr(pos_neg_bernoulli_bool x) { return x.fnr; }


/**
 * P[d(bernoulli0<T>(a) == a,bernoulli0<T>(a) == a)] = 1
 */
template <typename T, int N, struct partition_tag>
auto error_rate(int block, bernoulli_value<T,N,partition_tag> x)
{
    return x.err[block];
}

template <typename T, int N, struct partition_tag>
auto error_rate(int block, bernoulli_value<T,N,partition_tag> x)
{
    return x.err[block];
}


/**
 * P[bernoulli1<T>(a) == a]
 */
template <typeanme T>
auto error_rate(bernoulli1<T> x)
{
    return x.err;
}


auto error_rate(pos_neg_bernoulli2<T> x)
{
    return x.err2;
}

template <typename T, int N, struct partition_tag>
auto error_rate(int block, bernoulli<T,N,partition_tag> x)
{

}

auto fpr(bernoulli<bool,2,struct pos_neg> x)
{
    return x.fpr;
}

auto fnr(bernoulli_bool<2,struct pos_neg> x)
{
    return x.fnr;
}

template <typename S>
struct contains;

struct first_order_bernoulli_set
{
    double err;
    std::set<int> xs;

    using bernoulli_pred_type = contains<first_order_bernoulli_set>;
    using value_type = int;
    auto order() const { return 1; }

    auto operator()(int x) const { return xs.count(x) != 0; }
    void insert(int x) { xs.insert(x); }
};

auto error_rate(first_order_bernoulli_set x)
{
    return x.err;
}


/**
 * A set first-order Bernoulli has a characteristic probability given by
 *     P[d(contains(bernoulli_set1<T>(A),x),
 *         contains(bernoulli_set0<T>(a),x)) = 1 | x in T] = r
 * where d : (T,T) -> R is defined as
 *     d(a,b) := 0 if a == b otherwise 1.
 */



template <typename S>
struct contains
{
    using left_type = S;
    using right_type = typename S::value_type;

    auto operator()(
        left_type const & A,
        right_type const & x)
    {
        return bernoulli_bool<1,struct singleton_partition>{A(x),error_rate(A)};
    }
};



int main()
{
    first_order_bernoulli_set A{.1};
    A.insert(1);
    A.insert(2);

    auto p1 = first_order_bernoulli_set::bernoulli_pred_type{}(A,2);
    std::cout << p1.value << "\n";
    auto e1 = error_rate(p1);
    std::cout << e1 << "\n";

    auto p2 = first_order_bernoulli_set::bernoulli_pred_type{}(A,3);
    std::cout << p2.value << "\n";
    auto e2 = error_rate(p2);
    std::cout << e2 << "\n";
}