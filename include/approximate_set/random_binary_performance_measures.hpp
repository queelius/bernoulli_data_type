#pragma once

#include "first_order_rds.hpp"
#include "normal.hpp"
#include "binomial.hpp"

namespace alex::approximate_set
{
    /**
     * The true positive rate. This is the true positive rate
     * as a random variable given an expected true positive rate
     * of a.tpr() and p positives.
     * 
     * If p is unknown, a worst-case estimate is p=1.
     */
    template <typename X, typename R>
    normal tpr(first_order_rds<X,R> const & rds, double p)
    {
        return normal(rds.tpr(), rds.tpr()*rds.fnr()/p);
    };

    // The asymptotic distribution of the false positive rate.
    template <typename X, typename R>
    normal fpr(first_order_rds<X,R> const & a, double n)
    {
        return normal(rds.fpr(), rds.fpr()*rds.tnr()/n);
    };

    // The false negative rate.
    template <typename X, typename R>
    normal fnr(first_order_rds<X,R> const & rds, double p)
    {
        return normal(rds.fnr(), rds.tpr()*rds.fnr()/p);
    };

    // The true negative rate.
    template <typename X, typename R>
    normal tnr(first_order_rds<X,R> const & a, double n)
    {
        return normal(rds.tnr(), rds.fpr()*rds.tnr()/n);
    };

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