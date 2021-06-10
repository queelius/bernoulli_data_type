#pragma once

//#include "normal.hpp"
//#include "binomial.hpp"

namespace bernoulli::set::random
{
    template <typename T>
    struct set_indicator {};

    /**
     * The asymptotic distribution of the true positive rate.
     * The true positive rate is a random variable given an expected true
     * positive rate of a.tpr() and p positives.
     * 
     * This accepts a first-order bernoulli type over the set-indicator of T,
     * i.e., T is any object type in which the set-indicator on T makes sense,
     * e.g., std::set<X> and has been placed into the overload set.
     * 
     * If p is unknown, a reasonable worst-case estimate is p=1.
     * As p -> infinity, random_tpr(bs,p) converges in distribution to the point
     * bs.tpr(). Less realistically, as p -> 0, random_tpr(bs,p) converges in
     * distribution to an improper distribution with a uniform density.
     */
    template <typename P, typename T>
    auto random_tpr(bernoulli<first_order,T,set_indicator<T>> const & bs,
                    double p = 1)
    {
        return normal(bs.tpr(),bs.tpr()*bs.fnr()/p);
    }

    // The false positive rate.
    template <typename P, typename T>
    auto random_fpr(bernoulli<P,T,set_indicator<T>> const & bs, double n)
    {
        return normal(bs.fpr(),bs.fpr()*bs.tnr()/n);
    }
    
    // The false negative rate.
    template <typename P, typename T>
    auto random_fnr(bernoulli<P,T,set_indicator<T>> const & bs, double p)
    {
        return normal(bs.fnr(),bs.tpr()*bs.fnr()/p);
    }

    // The true negative rate.
    template <typename X, typename R>
    auto tnr(bernoulli<P,T,set_indicator<T>> const & bs, double n)
    {
        return normal(bs.tnr(),bs.fpr()*bs.tnr()/n);
    }

    /**
     * TPp / (p+n) + TNn / (p + n)
     * N((p*tpr(a) + n*tnr(a))/(p+n)
     */
    // The accuracy.
    template <typename T>
    auto accuracy(approximate_set<T> const & rds, double p, double n)
    {
        /*auto const & u = p + n;
        auto mu = (p*tpr(a) + n*tnr(a)) / u;
        auto var = (p*tpr(a)*(1.-tpr(a)) + (n*fpr(a)*(1.-fpr(a)))) / (u*u)
        return alex::stats::normal<double>(mu, var);
        */
       return 1.;
    };
        
    // The positive predictive value.
    template <typename T>
    auto ppv(approximate_set<T> const & a, real_interval lambda, double u)
    {
        return 1.;
        //return pow(1. + tp(a,lambda*u) / fp(a,(1.-lambda)*u),-1.);
    };

    // The negative predictive value.
    template <typename T>
    auto npv(approximate_set<T> const & a, real_interval lambda, double u)
    {
        return 1.;
    };
}