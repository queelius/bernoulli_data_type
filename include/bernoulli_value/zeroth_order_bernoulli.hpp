#pragma once

#include "bernoulli.hpp"

/**
 * data Bernoulli : a -> Vect n e -> Type where
 *     Zeroth  : Bernoulli a 0 e
 *     First : Bernoulli a 1 e
 *
 * complement : Bernoulli -> Bernoulli
 * complement Zeroth x = Zeroth 
 * 
 * 
 * 
 * Suppose f : X -> unit.
 * Then, 
 *     fmap f : bernoulli_value<N,X> -> bernoulli_value<0,unit>.
 * 
 * 
 * 
 * Bernoulli types are approximate, probabilistic data types.
 * The first-order Bernoulli type over T is denoted by
 *     bernoulli<1,T>
 * 
 * Suppose we have a sum type
 *     bernoulli<1,X> + bernoulli<1,Y>,
 * and we return a value
 *     x
 * of this type knows which of these two it is, i.e.,
 * if it is a value of type X then it has an error rate
 *     error_rate()
 * 
 * 
 * Consider the sum type
 *     (bernoulli a) + (bernoulli b)
 * 
 * For convenience, we may denote this type by
 *     bernoulli (a+b)
 * which is a second-order Bernoulli type.
 * If Y == Y1 == Y2, we may denote this by
 *     bernoulli<2,Y>,
 * and
 *     error_rate : (bernoulli<2,Y>,int) -> E
 * is defined as
 *     error_rate(x,0) := error_rate(left x)
 *  
 * where f applied to a given value of type X maps to either a value of type Y1 or Y2 and
 * provides the appropriate error measure.
 * 
 * 
 * 
 * Suppose f : X -> variant<Y1,Y2>.
 * Then,
 *     f : X -> variant<bernoulli<Y1>,bernoulli<Y2>>
 * where f applied to a given value of type X maps to either a value of type Y1 or Y2 and
 * provides the appropriate error measure.

 * We may instead denote this by
 *     bernoulli<2,variant<Y1,Y2>>.
 * Sometimes, Y1 = Y2 (which, by an accident in the C++ standard, is not
 * permitted), i.e.,
 *     bernoulli<2,variant<Y,Y>>,
 * 
 * 
 * 
 *  
 * 
 * Any Bernoulli type
 *     (X, = : X -> X -> bool, N)
 * denoted in C++ with
 * 
 *     template <regular_type X, int N>
 *     bernoulli<X,eq<X>,N>
 * 
 * where eq<X> is a function object that models an equality predicate for X,
 * e.g., by default:
 * 
 *     template <regular_type X>
 *     struct eq
 *     {
 *         bool operator==(X l, X r) { return l == r; }
 *     }
 * 
 * 
 * 
 * 
 * 
 * may be denoted by
 *     bernoulli_value<X,N>.
 * 
 * 
 * Any Bernoulli type
 *     (2^X, contains : X -> 2^X -> bool, P)
 * or
 *     bernoulli<set<X>, set<X>::contains, P>
 * may be denoted by
 *     bernoulli_set<X,P>
 *
 */


/**
 * Models the concept of a first-order Bernoulli structure over
 *     (bool, ==, 1).
 */

struct zeroth_order {};

template <typename T, typename F>
struct bernoulli<zeroth_order>
{
    using value_type = T;
    using fun = F;
    
    static auto order() { return 0; }
};

template <typename T, typename P>
using first_order_bernoulli = bernoulli<1,T,P>;

template <typename T, typename P>
using zeroth_order_bernoulli = bernoulli<0,T,P>;

template <typename S>
struct contains
{
    using set_type = S;
    using element_type = typename T::value_type;

    static bool operator()(set_type const & A, element_type const & a)
    {
        return A.contains(a);
    }
};

template <int N, typename S>
using bernoulli_set_indicator = bernoulli<N,T,contains<S>>;



template <typename T>
struct bernoulli<0,T>
{
    using value_type = T;
    static auto order() const { return 1; }

    T value;

    static double fpr() { return 0; }
    static double fnr() { return 0; }
    static double error(double) { return 0; }

    T operator() const { return value; }
    auto operator()() const { return value; }    
}

/**
 * A random_bernoulli<0,T> models a random variable that may be
 * sampled from. This one in particular models the zeroth-order bernoulli
 * model over values of type T, which is the degenerate case that has no error.
 */
template <typename T>
struct random_bernoulli<0,T>
{
    using value_type = T;    
    static int order() { return 0; }

    T given;

    static double fpr() const { return 0; }
    static double fnr() const { return 0; }
    static error(double) const { return 0; }

    auto operator()() const { return given; }

    template <typename R>
    auto sample(R & r) { return bernoulli<0,T>{given}; }
};


