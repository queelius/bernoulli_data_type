#pragma once

#include <functional>

template <size_t N, typename T>
struct random_bernoulli
{
    // (T, ==)
}

/**
 * A random Bernoulli value over
 *     (T, ==)
 * permits approximate queries on T over the equality predicate,
 *     P[bernoulli_value<N,T>{T(t)} != T(t) | T(t) in block i] = E[i],
 * where E is the exppected error rate and N is the order of the Bernoulli
 * approximation (the minimum number of blocks in a partition of T s.t. each
 * element in block j for j = 1, ..., N has a identically distributed Bernoulli
 * error rate).
 * 
 * In the Bernoulli model, the value T(t) is an uncertain latent variable and
 * bernoulli_value<N,T>{T(t)} is an observable value.
 *
 * If we are given T(t), then the probability that bernoulli_value<T>{T(t)}
 * takes on a specific value is given by the nature of the Bernoulli model over
 * T. For examples, consult the first-order random Bernoulli Boolean, denoted
 * by
 *     bernoulli<1,bool,universal>,
 * and second-order positive-negative random Bernoulli Boolean, denoted by
 *     bernoulli<2,bool,positive_negative>.
 *
 * There is usually much more structure in the distribution of Bernoulli values.
 * We are typically interested in the actual value bernoulli_value<N,T>{T(x)}
 * represents. Here, we know that it is T(x), but T(x) is typically latent in
 * the model and we only observe the Bernoulli value. Thus, we are interested
 * in
 *     P[bernoulli_value<N,T>{T(x)} == T(a)]
 * where
 *     P[bernoulli_value<N,T>{T(x)} == T(x)] = 1 - E.
 * 
 * Many Bernoulli values have a complicated structure, e.g., an entropy
 * Bernoulli map generates maps in which the probability that a value is
 * mapped correctly is a function of the implicit probability as implied by the
 * entropy coder.
 */



// Suppose we have a function of type X -> Y and we wish to apply it to values
// of type ber<1,X>. That is, we wish to lift f to a different function of
// type
//     ber<N,X> -> ber<?,Y>.
// In general, we cannot say what the output is, as it depends upon the
// injection. Regardless, we may provide a generic fmap such that
//     fmap : (X -> Y) -> ber<N,X> -> (ber<N,X> -> ber<-1,Y>),
// where order -1 indicates that we do not know the order and the error rate
// is given as uniformly being any value in the interval [0,1].
//
// For functions with particular properties, more can be said. For instance,
//     fmap : bijective(X -> Y) -> ber<N,X> -> (ber<N,X> -> ber<N,Y>),
// or
//     fmap : injective(X -> Y) -> ber<N,X> -> (ber<N,X> -> ber<N+1,Y>),
// where the error rate of f(x) is the same as the error rate of x.
//
// In C++, we provide fmap in an uncurried form, where fmap is a function
// objectthat takes a functional erasure, function<Y(X)>, and a bernoulli
// type, bernoulli<N,X>, and returns a value of type bernoulli<-1,Y>.
//
// Example:
// 
// Suppose f : X -> Y is bijective and we replace X with bernoulli<1,X,E>,
// then
//     P[bernoulli<1,X,E>{x} != X{x}] == E.
// When an error is produced, we do not say much about which value the Bernoulli
// realizes. The maximum entropy bernoulli<1,X,E> is such that
//     P[bernoulli<1,X,E>{a} == X{a}] == 1-E
// and
//     P[bernoulli<1,X,E>{a} == X{b}] == r/(|X|-1), a != b.
// However, if there is some non-binary loss function we may wish Bernoulli
// types to more likely realize values closer to the objective value.

template <typename X, typename Y, int N>
bernoulli<-1,Y> fmap(
    function<Y(X)> f,
    bernoulli<N,X> x)
{
    auto y = f(x.value);
    return bernoulli<-1,Y>{y};
}

template <typename X, typename Y, int N>
bernoulli<N,Y> fmap(
    bijective_function<Y(X)> auto & f,
    bernoulli<N,X> x)
{
    auto y = f(x.value);
    return bernoulli<N,Y>{y};
}

/**
 * Observe that, if
 *     f : X \-> Y
 * is partial and we apply fmap to f such that
 *     fmap(f) : bernoulli<N,X> -> bernoulli<M,Y>,
 * then some subset of dod(f) is mapped to some subset of complement(dod(f)) and
 * vice versa.
 * 
 * In other words, supposing we start with elements in dod(f) in the objective
 * system and apply f to it, in the Bernoulli approximation undefined behavior
 * may result.
 */

