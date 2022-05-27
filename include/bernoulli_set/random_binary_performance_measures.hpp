#pragma once

//#include "normal.hpp"
//#include "binomial.hpp"

namespace bernoulli::set::random
{
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
    template <typename T,2>
    auto random_tpr(bernoulli_set<T,2> const & s,
                    double p = 1)
    {
        return normal(s.tpr(),s.tpr()*s.fnr()/p);
    }

    // The asymptotic distribution of the false positive rate.
    template <typename T,2>
    auto random_fpr(bernoulli_set<T,2> const & s, double n)
    {
        return normal(s.fpr(),s.fpr()*s.tnr()/n);
    }
    
    // The asymptotic distribution fo teh false negative rate.
    template <typename T,2>
    auto random_fnr(bernoulli<T,2> const & s, double p)
    {
        return normal(s.fnr(),s.tpr()*s.fnr()/p);
    }

    // The asymptotic distribution of the true negative rate.
    template <typename T>
    auto random_tnr(bernoulli<T,2> const & s, double n)
    {
        return normal(s.tnr(),s.fpr()*s.tnr()/n);
    }

    /**
     * The asymtotic distribution of the accuracy.
     */
    template <typename T>
    auto random_accuracy(bernoulli_set<T,2> const & s, double p, double n)
    {
        auto u = p + n;
        auto mu = (p*s.tpr() + n*s.tnr())/u;
        auto var = (p*s.tpr()*(1-s.tpr()) + (n*s.fpr()*(1-s.fpr()))) / (u*u)
        return normal(mu, var);
    };
    
    // The asymptotic distribution of the positive predictive value (ppv).
    template <typename T>
    auto random_ppv(bernoulli_set<T,2> const & a,
             interval<double> lambda,
             double u)
    {
        throw "not implemented yet"
    }

    // The asymptotic distribution of the negative predictive value (npv).
    template <typename T>
    auto random_npv(bernoulli_set<T,2> const & s,
                    interval<double> lambda,
                    double u)
    {
        throw "not implemented yet"
    }
}