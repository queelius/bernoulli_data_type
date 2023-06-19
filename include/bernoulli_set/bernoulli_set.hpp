#pragma once

/**
 * @file bernoulli_set.hpp Type-erased set open over types and closed over
 * some specified set of operators.
 * 
 * bernoulli_set<X,N> models the concept of a Nth order Bernoulli set with
 * elements of type X by wrapping some type that models the concept and then
 * subsequently erases the specific type. This is known as type-erasure, and
 * allows, for instance, one to store Bernoulli sets that vary over types into
 * a container of bernoulli_set<X,N> elements.
 * 
 * As a model of a set type, set<T> provides a predicate
 *     contains : (set<T>, T) -> bool
 * to answer queries about set membership.
 * 
 * There are many ways to represent a set, both mathematically and
 * representationally in computer memory. Depending upon the particulars of the
 * implementation, there may be functions, like union, that have more efficient
 * algorithms than the general case.
 */

#include <memory>

/**
 * Models `bernoulli<set<X>,N>`.
 *
 * Example: `bernoulli_set<X,1>` models the concept of a binary symmetric
 *          channel.
 *
 * As a degenerate case, `bernoulli_set<X,0>` models the concept of a `set<X>`.
 * Note that `set<X>` is not `std::set<X>`, but another type that models the
 * concept of a set over `X`.
 *
 * Note that `bernoulli_set<X,N>` is *non-iterable*, even though
 * concrete types that model Bernoulli sets may be iterable.
 *
 * `bernoulli_set<X,N>` is not a regular type since for simplicity we decided
 * not to implement the equality predicate (which in turn may also return a
 * Bernoulli Boolean).
 *
 * If we wished to implement equality, we might take the following approach.
 * Given two Bernoulli sets over `X`, consider the following two facts:
 *
 * (1) The probability that they are equal goes to zero as
 *     |X| goes to infinity.
 *
 * (2) Representational equality implies equality.
 * 
 * For a finite univeral set, exact equality can be implemented, but as an
 * approximation, two Bernoulli sets are equal iff they have the same
 * representation. We can store a hash of the representation to compute
 * equality in `O(1)` time, i.e., `hash(rep(s1)) == hash(rep(s2))`.
 * It is interesting to point out that even if representational equality
 * implied equality and equality implied representational equality, then
 * hashing the representations would generate an equality
 * predicate of type
 * ```
 *     == : (bernoulli_set<X,N>,bernoulli_set<X,N>) -> bernoulli<1,bool,eq>
 * ```
 * such that the false positive and false negative rates would both be given by
 * `2^(-k)` where `k` is the number of bits `hash` generates, assuming `hash`
 * models a crypographic hash function.
 *
 * In this case, we see that the Bernoulli set models
 * a Bernoulli type whose equality predicate is given by
 * ```
 *     == : (bernoulli_set<X,N>,bernoulli_set<X,N>) -> bernoulli<bool,1>
 * ```
 * and whose membership predicate is given by
 * ```
 *     is_member : (X,bernoulli_set<X,N>) -> bernoulli<bool,2>
 * ```
 *
 * Of course, we can generate higher-order Bernoulli types by replacing `X`
 * with `bernoulli<X,K>
 */

template <typename X>
class bernoulli_set: public algebra::bernoulli_set_expr<bernoulli_set<X>>
public:
    
    using value_type = X;

    template <typename B>
    bernoulli_set(B const & s)
        : s_(std::make_shared<model<B> const>(s)) {};

    bernoulli_set(bernoulli_set const & s) : s_(s._s) {};

    auto contains(X const & x) const { return s_->contains(x); }
    auto operator()(X const & x) const { return contains(x); }
    auto false_positive_rate() const { return s_->fpr(); }
    auto false_negative_rate() const { return s_->fnr(); }

private:
    struct concept_
    {
        virtual bernoulli_bool contains(X const &) const = 0;
        virtual interval<double> fpr() const = 0;
        virtual interval<double> fnr() const = 0;
    };

    template <typename B>
    struct model_ final : concept_
    {
        model(B s) : s_(s) {}

        bernoulli_bool contains(T const & x) const { return s_->contains(x); }
        interval<double> fpr() const { return s_->false_positive_rate(); }
        interval<double> fnr() const { return s_->false_negative_rate(); }

        B s_;
    };

    std::shared_ptr<concept_ const> s_;
};


