#include "random_bernoulli.hpp"

template <>
struct bernoulli<1,bool>
{
    double err;
}

/**
 * Suppose we have a data type X and a data type T that models sets over X and
 * it is equipped with the predicate
 *     contains : (T,X) -> bool,
 * then we may perform membership tests for elements of type X for sets modeled by T over 
 * we refer to this structure as 
 *     
 * The Bernoulli set model,
 *     (T, contains : (T,X) -> bool)
 */

auto operator~(bernoulli<1,bool> a)
{
    return bernoulli<1,bool>{!a.value,a.err};
}

auto operator~(bernoulli<2,bool> a)
{
    return bernoulli<2,bool>{!a.value,a.fpr,a.fnr};
}

template <int N>
auto operator~(bernoulli<N,bool> a)
{
    return bernoulli<N,bool>{!a.value,a.fpr,a.fnr};
}


auto operator&(bernoulli<1,bool> a, bernoulli<1,bool> b)
{
    // three ways a false positive may occur
    auto const fpr1 = a.err * (1 - b.err);
    auto const fpr2 = b.err * (1 - a.err);
    auto const fpr3 = a.err * b.err;

    // one way a false negative may occur
    auto const fnr = 1-(1-a.err)*(1-b.err);

    // the result is order bernoulli<4,bool> but any order larger than 2
    // we treat with minimum spans and lose information (since the a priori
    // joint distributions are probably unknown, this loss of info in practice
    // is irrelevant).
    return bernoulli<-1,bool>{
        a.value && b.value,
        min_span(fpr1,fpr2,fpr3),
        min_span(fnr,fnr)
    };
}

auto operator&(bernoulli<2,bool> a, bernoulli<2,bool> b)
{
    // three ways a false positive may occur
    auto const fpr1 = a.fpr * (1 - b.fnr);
    auto const fpr2 = b.fpr * (1 - a.fnr);
    auto const fpr3 = a.fpr * b.fpr;

    // one way a false negative may occur
    auto const fnr = 1-(1-a.fnr)*(1-b.fnr);
    
    // the result is order bernoulli<4,bool> but any order larger than 2
    // we treat with minimum spans and lose information (since the a priori
    // joint distributions are probably unknown, this loss of info in practice
    // is irrelevant).
    return bernoulli<-1,bool>{
        a.value && b.value,
        min_span(min(fpr1,min(fpr2,fpr3)),max(fpr1,max(fpr2,fpr3))),
        min_span(fnr,fnr)
    };
}

/**
 * 
 */
auto operator&(bernoulli<-1,bool> a, bernoulli<-1,bool> b)
{
    auto fpr1 = a.fpr * (1 - b.fnr);
    auto fpr2 = b.fpr * (1 - a.fnr);
    auto fpr3 = a.fpr * b.fpr;

    auto fnr = 1 - (1 - a.fnr) * (1 - b.fnr);

    return bernoulli<-1,bool>{
        a.value && b.value,
        min_span(fpr1,fpr2,fpr3),
        fnr
    };
}
