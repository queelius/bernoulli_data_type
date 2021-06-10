
/**
 * Let T := {a} + {b} + {c} = {a,b,c}.
 * 
 * The set of partitions of {a,b,c} is given by
 *     { {{a},{b},{c}},
 *       {{a,b},{c}}
 *       {{a,c},{b}}
 *       {{b,c},{a}}
 *       {{a,b,c}} }
 * and the number of unique partition sizes is 1, 2, and 3.
 * 
 * We denote a particular value x of type T with T(x), e.g., T(a).
 * 
 * Suppose we have the identity function id : T -> T but with some added
 * random noise, denoted noisy_id : T -> (noisy T) such that
 *     P[noisy_id(a) != a] = error_rate(a)
 * and therefore
 *     P[nosiy_id(a) == b] + noisy_id(a) == c] = 1 - error_rate(a).
 * 
 * Similiarly,
 *     P[noisy_id(b) != b] = error_rate(b),
 * and
 *     P[noisy_id(c) != c] = error_rate(c).
 * 
 * Further, suppose that
 *     P[noisy_id(a) != a | noisy_id(b) != b] = P[noisy_id(a) != a]
 * and likewise for elements b and c, i.e., they are independent events.
 *
 * When we are only interested in the events
 *     [noisy_id(x) != x]
 * and
 *     [noisy_id(x) == x]
 * for each x in T, then noisy_d applied to each element of T has two possible
 * outcomes and therefore each one is a independent Bernoulli trial.
 * 
 * Let R := {error_rate(a),error_rate(b),error_rate(c)}. Then, we denote
 * that a value type T realizes uncertain values according to the above
 * formulation with
 *     (bernoulli T |R|).
 * 
 * Let U := {0,1}. The Cartesian product of T and U is given by
 *     (T,U) := { (a,0), (a,1), (b,0), (b,1), (c,0), (c,1) }
 * which has Bell(6) = 52 ways to partition.
 * 
 * Again, let us consider noisy_id : (T,U) -> (noisy (T,U)) such that
 *     P[noisy_id((a,0)) != (a,0)] = error_rate((a,0))
 * and so on for remaining elements of (T,U).
 *     
 * 
 * 
 * 
 */


template <typename T, int L, int H>
struct partition
{
    using value_type = T;

    static constexpr min_order() { return L; }
    static constexpr max_order() { return H; }
    static constexpr order() { return std::pair<int>{min_order(),max_order()}; }

    auto block(T const & t) const
    {
        return std::make_pair<int>(1,H);
    }
};

// note that, in reality, the order of a partition must be between 1 and
// cardinality(T), but we do not bother to keep track of this information.
template <typename T>
using unknown_partition = partition<T,0>;

// we know the exact size of the partition.
template <typename T, typename P, typename K>
using kpartition = partition<T,K,K>;

template <equality_comparable T>
struct bernoulli_value<unkown_partition<T>>
{
    using value_type = T;

    T value;
    interval err;

    auto order() const { return unknown_partition<T>::order(); }

    // we know nothing about the error rate for any block nor do we know the
    // number of blocks, but we have some minimum span over error rates of all
    // the blocks.
    auto error_rate(int) const { return err; }

    // the span of all the error rates over all the blocks is (0,1) also.
    auto error_rate_span() const { return err; }
};

// we only know the order of the bernoulli, in which T is partitioned into
// K blocks where each block has an independent and unique error rate.
// We do not know, however, the particular partition of size K.
template <equality_comparable T, int K>
struct bernoulli_value<npartition<T,K>>
{
    using value_type = T;

    T value;
    std::array<interval,K> err;

    auto order() const { return K; }

    auto error_rate(int k) const { return err[k]; }

    // the span of the error rates over all the blocks
    auto error_rate_span() const { return min_span(err); }
};

/**
 * The equality predicate is the characteristic predicate for bernoulli values.
 * That is, P[bernoulli_value<T,P>(x) != T(x)] = error_rate(k,x) where k is the
 * block that element x is in.
 * 
 * Often, we do not know which block an element belongs to, in which case a
 * minimum span over all the relevant blocks is returned.
 */
template <equality_comparable T, int K1, int K2>
auto operator==(
    bernoulli_value<npartition<K>> const & lhs,
    T rhs
{
    return bernoulli_value<T,npartition<K>>{lhs.value == rhs, lhs.err};
}

template <equality_comparable T, int K>
auto operator==(
    T lhs,
    bernoulli_value<npartition<K>> const & rhs,
{
    return rhs == lhs;
}

template <typename T, int K1, int K2>
auto operator==(
    bernoulli_value<T,npartition<K1>> const & lhs,
    bernoulli_value<T,npartition<K2>> const & rhs,
{
    // here, we are only interested in whether two observations of a
    // bernoulli value are equal. we do not carry the error rate forward.
    return lhs.value == rhs.value;
}


struct bernoulli_value<bool,npartition<1>>
{
    using value_type = bool;

    bool value;
    interval err;

    auto order() const { return 1; }
    auto error_rate(int) const { return err; }
    auto error_rate_span() const { return err; }

    auto operator==(bool x) const
    {
        return bernoulli_value<bool,npartition<1>>{value == x, err};
    }
};

using bernoulli_bool1 = bernoulli_value<bool,npartition<1>>;
using bernoulli_bool2 = bernoulli_value<bool,npartition<2>>;

// if w take the union of two bernoulli sets of order 2,
// then it is a bernoulli set of order 2 with an uncertainty that is the
// min span as given in the paper.
//
// complement is order 2



// Suppose we have a function of type X -> Y and we wish to provide as input a
// Bernoulli value of type X instead. We provide an overloaded set of functors,
// denoted fmap, that automatically lifts functions to functions that take
// Bernoulli value types,
//     fmap : X -> Y -> bernoulli<X,P> -> bernoulli<Y,Q>
// where in general P != Q. Since the function's mapping is
// not generally specified at the type level, we do not generally know P and
// and Q. The generic fmap is then denoted by
//     fmap : X -> Y -> bernoulli_value<X,P> -> bernoulli<Y,unknown>
// where the partition type denoted by unknown knows absolutely nothing about
// the partition.
//
// If we know more about a function then more information about its lifted type
// can be inferred, e.g.,
//     fmap : injective(X->Y) -> bernoulli<X,P> -> bernoulli<Y,npartition<K>()>
// where
//     K := bernoulli<X,P>::order()
// and
//     npartition<K>
// is a partition strategy in which the only thing known about it is where the order of the output is the same as the order of the input the partition is generally unknown.




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