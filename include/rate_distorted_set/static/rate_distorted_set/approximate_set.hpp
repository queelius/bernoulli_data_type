/**
 * rate_distorted_set<X,W,F> models a rate-distorted
 * set (RDS) of type X with an expected false negative rate W and an expected
 * false positive rate E.
 * 
 */

#pragma once

#include <memory>
#include "interval.hpp"
#include "tl.hpp"
#include <tuple>
#include <limits>
#include <variant>

namespace alex::set::rate_distorted
{
    struct complement_of {};
    struct union_of {};
    struct disjoint_union_of {};
    struct cartesian_product_of {};
    struct intersection_of {};
    struct empty {};
    struct universal {};

    template <typename X, typename R>
    struct first_order_approximate_set_concept
    {
        using rate_type = R;
        virtual bool contains(X const &) const = 0;
        virtual R fpr() const = 0;
        virtual R fnr() const = 0;
    };

    template <typename T>
    class first_order_approximate_set
    {
    public:
        using value_type = T;
        using rate_type = alex::math::interval<double>;

        rdset(empty) : self_(std::make_shared<approximate_set_empty>()) {};
        rdset() : self_(std::make_shared<approximate_set_empty>()) {};
        rdset(universal) : self_(std::make_shared<approximate_set_universal>()) {};

        template <typename ASet>
        approximate_set(ASet a) : self_(std::make_shared<approximate_set_wrapper<ASet>>(std::move(a)))
        {
            // do nothing else        
        };

        template <typename L, typename R>
        approximate_set(L l, R r, union_of) : self_(std::make_shared<approximate_set_union<L,R>>(std::move(l), std::move(r)))
        {
            // do nothing else        
        };

        template <typename L, typename R>
        approximate_set(L l, R r, intersection_of) : self_(std::make_shared<approximate_set_intersection<L,R>>(std::move(l), std::move(r)))
        {
            // do nothing else        
        };

        template <typename ASet>
        approximate_set(ASet a, complement_of) : self_(std::make_shared<approximate_set_complement<ASet>>(std::move(a)))
        {
            // do nothing else        
        };

        bool contains(T const & x) const
        {
            return self_->contains(x);
        };

        rate_type fpr() const
        {
            return self_->fpr();
        };

        rate_type tpr() const
        {
            return self_->tpr();
        };

    private:
        struct approximate_set_empty final : approximate_set_concept<T>
        {
            rate_type fpr() const override { return rate_type(0,0); };            
            rate_type tpr() const override { return rate_type(1,1); };            
            bool contains(T const &) const override { return false; };
        };

        struct approximate_set_universal final : approximate_set_concept<T>
        {
            rate_type fpr() const override { return rate_type(0,0); };            
            rate_type tpr() const override { return rate_type(1,1); };            
            bool contains(T const &) const override { return true; };
        };

        template <typename L, typename R>
        struct approximate_set_union final : approximate_set_concept<T>
        {
            approximate_set_union(L l, R r) : l(std::move(l)), r(std::move(r)) {};

            rate_type fpr() const { return 1.-(1.-l.fpr())*(1.-r.fpr()); };

            rate_type tpr() const
            {
                return 1. - alex::math::min_span(
                {
                    (1.-l.tpr())*(1.-r.tpr()),
                    (1.-l.tpr())*(1.-r.fpr()),
                    (1.-r.tpr())*(1.-l.fpr())
                });
            };

            bool contains(T const & x) const { return l.contains(x) || r.contains(x); };

            L l;
            R r;
        };

        template <typename L, typename R>
        struct approximate_set_intersection final : approximate_set_concept<T>
        {
            approximate_set_intersection(L l, R r) : l(std::move(l)), r(std::move(r)) {};
            rate_type fpr() const { return rate_type(0,0); };
            rate_type tpr() const
            {
                return rate_type(1,1);
            };
            bool contains(T const & x) const { return l.contains(x) && r.contains(x); };

            L l;
            R r;
        };

        template <typename ASet>
        struct approximate_set_complement final : approximate_set_concept<T>
        {
            approximate_set_complement(ASet a) : a(std::move(a)) {};
            rate_type fpr() const override { return 1. - a.tpr(); };
            rate_type tpr() const override { return 1. - a.fpr(); };
            bool contains(T const & x) const override { return !a.contains(x); };            
            ASet a;
        };

        template <typename ASet>
        struct approximate_set_wrapper final : approximate_set_concept<T>
        {
            approximate_set_wrapper(ASet a) : a(std::move(a)) {};
            bool contains(T const & x) const override { return a.contains(x); };
            rate_type fpr() const override { return false_positive_rate(a); };
            rate_type tpr() const override { return true_positive_rate(a); };
            ASet a;
        };

        template <typename L, typename R>
        struct approximate_set_disjoint_union final : approximate_set_concept<T>
        {
            approximate_set_disjoint_union(L l, R r) :
                l(std::move(l)), r(std::move(r)) {};

            rate_type fpr() const { return min_span(l.fpr(),r.fpr()); };

            rate_type tpr() const { return min_span(l.tpr(),r.tpr()); };

            bool contains(T const & x) const
            {
                return l.contain(x) || r.contain(x);
            };

            L l;
            R r;
        };

        /**
         * fpr<X> : ASet<T> -> 2^[0,1]
         * 
         * If X convertible to T, then returns a false positive rate for that
         * value type. Note that if T is a sum type (variant), then fpr<X>
         * returns the false positive rate for type Xalternative, which is independent of
         * the false positive rate of other (disjoint) alternatives.
         * 
         * If X is not convertible to T, then returns degenerate interval [0,0],
         * which only has member 0.
         * 
         * There is a non-templated version, also.
         * 
         * fpr : ASet<T> -> 2^[0,1]
         * 
         * Returns an interval that contains the false positive rate for any
         * type convertible to T. The biggest difference is with the sum type,
         * which returns the minimum span of all of false positive rate for
         * all of the types in T.
         */

        template <typename L, typename R>
        struct approximate_set_cartesian_product final : approximate_set_concept<T>
        {
            approximate_set_cartesian_product(L l, R r) :
                l(std::move(l)), r(std::move(r)) {};

            rate_type fpr() const { return l.fpr() * r.fpr(); };

            rate_type tpr() const { return l.tpr() * r.tpr(); };

            bool contains(T const & x) const
            {
                return l.contains(std::get<0>(x)) &&
                       r.contains(std::get<1>(x));
            };

            L l;
            R r;
        };

        std::shared_ptr<approximate_set_concept const> self_;
    };

    template <typename S> // S models an approximate_set<value_type(S)>
    auto make_approximate_complement(S a)
    {
        return approximate_set<typename S::value_type>(std::move(a), complement_of{});
    };

    template <
        typename L, // L models an approximate_set<value_type(L)> and
        typename R> // R models an approximate_set<value_type(R)> where value_type(R) is
                    // convertible to value_type(L)
    auto make_approximate_union(L l, R r)
    {
        return approximate_set<typename L::value_type>(std::move(l), std::move(r), union_of{});
    };

    template <typename ASet>
    auto make_approximate_set(ASet a)
    {
        return approximate_set<typename ASet::value_type>(std::move(a));
    };

    template <typename L, typename R>
    auto make_approximate_intersection(L l, R r)
    {
        return approximate_set<typename L::value_type>(std::move(l), std::move(r), intersection_of{});
    };

    template <typename T>
    auto make_universal_set()
    {
        return approximate_set<T>(universal{});
    };

    template <typename T>
    auto make_empty_set()
    {
        return approximate_set<T>(empty{});
    };

    template <
        typename L, // L models an approximate_set<value_type(L)>
        typename R> // R models an approximate_set<value_type(R)>
    auto make_approximate_cartesian_product(L l, R r)
    {
        using type_list_left = decltype(as_type_list((typename L::value_type){}));
        using type_list_right = decltype(as_type_list((typename R::value_type){}));
        using type_list = append_t<type_list_left,type_list_right>;
        using value_type = decltype(as_tuple(type_list{}));

        return approximate_set<value_type>(std::move(l), std::move(r), cartesian_product_of{});
    };

    template <
        typename L, // L models an approximate_set<value_type(L)>
        typename R> // R models an approximate_set<value_type(R)>
    auto make_disjoint_union(L l, R r)
    {
        using type_list_left = decltype(as_type_list((typename L::value_type){}));
        using type_list_right = decltype(as_type_list((typename R::value_type){}));
        using type_list = append_t<type_list_left,type_list_right>;
        using value_type = decltype(as_variant(type_list{}));

        return approximate_set<value_type>(std::move(l), std::move(r), disjoint_union_of{});
    };


template <typename ASet>
class PowerSet
{
public:
    PowerSet(ASet a) : a(a) {};

    using value_type = std::set<typename ASet::value_type>;

    bool contains(value_type const & s) const
    {
        for (const auto& x : s)
            if (!a.contains(x)) return false;
        return true;
    };

    bool operator==(const PowerSet<ASet>& ps)
    {
        return a == ps.a;
    };

    interval fpr() const
    {
        return 0;
    };

    interval tpr() const
    {
        return 1;
    };

private:
    ASet a;
};

template <
    typename L,
    typename R
>
class CartesianProduct
{
public:
    CartesianProduct(L a, R b) : a(a), b(b) {};

    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::pair<left_value_type,right_value_type>;

    bool contains(value_type const & p) const
    {
        return a.contains(p.first) && b.contains(p.second);
    };

    interval fpr() const
    {
        return 0;
    };

    interval tpr() const
    {
        return 1;
    };

    bool operator==(const CartesianProduct<L,R>& other)
    {
        return a == other.a && b = other.b;
    };

    double cardinality() const
    {
        return cardinality(p.left) * cardinality(p.right);
    };

    auto left_project() const
    {
        return p.left;
    };

    auto right_project() const
    {
        return p.right;
    };

    auto flip() const
    {
        return CartesianProduct<Right,Left>(b, a);
    };

private:
    L a;
    R b;
};

template <
    typename L,
    typename R
>
class DiscrimantedUnion
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::variant<
        left_value_type,
        right_value_type>;

    DiscrimantedUnion(L a, R b) : a(a), b(b) {};

    bool contains(value_type const & x)
    {
        if (x is same as left_value_type)
            return a.contains(x) || b.contains(x);
    };

    interval fpr() const
    {
        return one() - (one() - a.fpr()) * (one() - b.fpr());
    };

    interval fnr() const
    {
        return span(
            a.fnr() * b.fnr(),
            span(
                a.fnr()*(one()-b.fpr()),
                b.fnr()*(one()-a.fpr())));
    };

    bool operator==(DiscrimantedUnion<L,R> const & other)
    {
        return left == other.a && b == other.b;
    };

private:
    L left;
    R right;    
};

template <
    typename L, // L models the concept of random approximate sets
    typename T 
>
auto
make_union(L left, EmptySet<T> right)
{
    return left;
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
    typename L,
    typename R
>
auto
make_discriminated_union(L left, R right)
{
    return DiscriminatedUnion<L,R>(left,right);
}

template <
    typename T,
    typename R
>
auto
make_discriminated_union(EmptySet<T> left, R right)
{
    return right;
};

template <
    typename L,
    typename T
>
auto
make_discriminated_union(L left, EmptySet<T> right)
{
    return left;
};

template <
    typename T,
    typename R
>
auto
make_intersection(EmptySet<T> left, R right)
{
    return left;
}

template <
    typename T,
    typename R
>
auto
make_intersection(EmptySet<T> left, R right)
{
    return left;
};

template <
    typename T,
    typename R
>
auto
make_cartesian_product(EmptySet<T> left, R right)
{
    return left;
};

template <
    typename L,
    typename T
>
auto
make_cartesian_product(L left, EmptySet<T> right)
{
    return right;
};

template <
    typename L,
    typename R
>
auto
make_cartesian_product(L left, R right)
{
    return CartesianProduct<L,R>(left,right);
};



}






// do we want to call this an algebraic data type? a data type is a set an elements
// of the set are its values.
// 

// what is the probability that a particular relation R <= A x B
// is a subset of A* x B* also? suppose A x B is a subset of U x U.
// then, A* and B* are also subsets of U.
//
// given a relation R in A x B, an approximate relation R* in A x B
// is just an approximate set where the members are pairs.

using interval = std::pair<double, double>;

interval one() { return interval(1,1); };
interval zero() { return interval(0,0); };

interval span(const interval& a, const interval& b)
{
    return interval(
        std::min(a.first, b.first),
        std::max(a.second, b.second));
};





namespace alex::random_approximate_set
{
using interval = std::pair<double, double>;

interval one() { return interval(1,1); };
interval zero() { return interval(0,0); };

interval span(const interval& a, const interval& b)
{
    return interval(
        std::min(a.first, b.first),
        std::max(a.second, b.second));
};


template <
    typename T,                     // sets are elements of type T
    template <typename> typename L, // L models an approximate set type
    template <typename> typename R, // R models an approximate set type
                                    // Note: L and R should be parameterized by type T
                                    //       for union to make sense. See DisjointUnion
                                    //       to relax this constraint.
    typename I = interval           // I models an interval type
>
class Union
{
public:
    using value_type = T;
    using interval_type = I;

    Union(L const & a, R const & b) : a(a), b(b) {};

    bool contains(T const & x)
    {
        return a.contains(x) || b.contains(x);
    };

    I fpr() const
    {
        return one() - (one() - a.fpr()) * (one() - b.fpr());
    };

    I fnr() const
    {
        return span(
            a.fnr() * b.fnr(),
            span(
                a.fnr()*(one()-b.fpr()),
                b.fnr()*(one()-a.fpr())));
    };

    bool operator==(Union<T,L,R,I> const & other)
    {
        return a == other.a && b == other.b;
    };

private:
    L<T> a;
    R<T> b;    
};

template <
    typename T,
    template <typename> typename L,
    template <typename> typename R,
    typename I = interval
>
class Intersection
{
public:
    using value_type = T;
    using interval_type = I;

    Intersection(L const & a, R const & b) : a(a), b(b) {};

    bool contains(T const & x)
    {
        return a.contains(x) && b.contains(x);
    };

    I fpr() const
    {
        return one() - (one() - a.fpr()) * (one() - b.fpr());
    };

    I fnr() const
    {
        return span(
            a.fnr() * b.fnr(),
            span(
                a.fnr()*(one()-b.fpr()),
                b.fnr()*(one()-a.fpr())));
    };

    bool operator==(Intersection<T,L,R,I> const & other)
    {
        return a == other.a && b == other.b;
    };

private:
    L<T> a;
    R<T> b;    
};


template <
    typename L,
    typename R,
    typename I = interval
>
class DisjointUnion
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::variant<
        left_value_type,
        right_value_type>;

    DisjointUnion(L a, R b) : a(a), b(b) {};

    bool contains(value_type const & x)
    {
        return x.index() == 0 ?
            a.contains(x) : b.contains(x);
    };

    I fpr() const
    {
        return span(left.fpr(), right.fpr());
    };

    I fnr() const
    {
        return span(left.fnr(), right.fnr());;
    };

    bool operator==(DisjointUnion<L,R,I> const & other)
    {
        return left == other.a && b == other.b;
    };

    auto cardinality() const
    {
        return cardinality(left) + cardinality(right);
    };

private:
    L left;
    R right;    
};


template <typename ASet>
class PowerSet
{
public:
    PowerSet(ASet a) : a(a) {};

    using interval_type = typename ASet::interval_type;
    using value_type = std::set<typename ASet::value_type>;

    bool contains(value_type const & s) const
    {
        for (const auto& x : s)
            if (!a.contains(x)) return false;
        return true;
    };

    bool operator==(const PowerSet<ASet>& ps)
    {
        return a == ps.a;
    };

    auto cardinality() const
    {
        return std::pow(2., a.cardinality());
    };

    interval_type fpr() const
    {
        return {0,0};
    };

    interval_type fnr() const
    {
        return {0,0};
    };

private:
    ASet a;
};

template <
    typename L,
    typename R
>
class CartesianProduct
{
public:
    CartesianProduct(L const & a, R const & b) : a(a), b(b) {};

    using left_value_type = typename Left::value_type;
    using right_value_type = typename Right::value_type;
    using value_type = std::pair<left_value_type, right_value_type>;
    using interval_type = typename Left::interval_type;

    bool contains(value_type const & p) const
    {
        return a.contains(p.first) && b.contains(p.second);
    };

    interval_type fpr() const
    {
        return 0;
    };

    interval_type tpr() const
    {
        return 1;
    };

    bool operator==(const CartesianProduct<L,R>& other)
    {
        return a == other.a && b = other.b;
    };

    auto cardinality() const
    {
        return cardinality(a) * cardinality(b);
    };

    auto left_project() const
    {
        return p.left;
    };

    auto right_project() const
    {
        return p.right;
    };

    auto converse() const
    {
        return CartesianProduct<R,L>(b, a);
    };

private:
    L a;
    R b;
};



/*
    // A probability_type is a probability and
    // thus should take on a value between 0 and 1.
    typedef double probability_type;
    
    // The cardinality_type is able to represent
    // the cardinality of sets. Since an approximate
    // set may have an uncertain number of false
    // positives or true positives, the
    // cardinality_type is a real number type.
    typedef double cardinality_type;
    
    // The value_type is the type of elements
    // in the set.
    typedef X value_type;
    
    // If x is in the approximated set S, returns
    // true with probability
    //     1 - false_negative_rate().
    // Otherwise, returns true with probability
    //     false_positive_rate().
    virtual
    bool contains(const X& x) const = 0;
    
    // The cardinality of the approximate set.
    // If the approximate set has an uncertain number
    // of false positives or true positives, the
    // expected cardinality may be returned.
    virtual
    cardinality_type cardinality() const = 0;
    
    // Returns the false positive rate,
    // the probability that an element
    // not in the approximated set S
    // is in the approximate set S*.
    virtual
    probability_type false_positive_rate() const = 0;

    // Returns the false negative rate,
    // the probability that an element
    // in the approximated set S
    // is not in the approximate set S*.
    virtual
    probability_type false_negative_rate() const = 0;
*/
};
