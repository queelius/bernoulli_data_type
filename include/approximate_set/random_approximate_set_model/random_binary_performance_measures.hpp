#include "approximate_set.hpp"
#include "interval.hpp"
#include "normal.hpp"
//#include "binomial.hpp"

namespace alex::approximate_set
{
    using real_interval = alex::math::real_interval;
    using normal = alex::stats::normal<double>;

    template <typename T>
    normal tp(approximate_set<T> const & a, double p = 1)
    {
        return normal(p*a.tpr(), p*a.tpr()*(1-a.tpr()));
    };

    template <typename T>
    normal fp(approximate_set<T> const & a, double n = 1)
    {
        return normal(n*a.fpr(), n*a.fpr()*tnr(a)));
    };

    template <typename T>
    normal tn(approximate_set<T> const & a, double n = 1)
    {
        return normal(p*tnr(a), n*tnr(a)*(1-a.fpr()));
    };

    template <typename T>
    normal fn(approximate_set<T> const & a, double p = 1)
    {
        return normal(p*fnr(a), p*fnr(a)*fnr(a));
    };

    /**
     * The true positive rate. This is the true positive rate
     * as a random variable given an expected true positive rate
     * of a.tpr() and p positives.
     * 
     * If p is unknown, a worst-case estimate is p=1.
     */
    template <typename T>
    normal tpr(approximate_set<T> const & a, double p = 1)
    {
        return normal(a.tpr(), a.tpr()*(1-a.tpr())/p);
    };

    // The false positive rate.
    template <typename T>
    normal fpr(approximate_set<T> const & a, double p = 1)
    {
        return normal(a.fpr(), a.fpr()*(1-a.fpr())/n);
    };

    // The false negative rate.
    template <typename T>
    normal fnr(approximate_set<T> const & a, double p)
    {
        return normal(1.-a.tpr(), a.tpr()*(1-a.tpr())/p);
    };

    // The true negative rate.
    template <typename T>
    normal tnr(approximate_set<T> const & a, double n)
    {
        auto tnr_ = 1.-a.fpr();
        return normal(1.-a.fpr(),a.fpr()*(1-a.fpr())/n);
    };

    /**
     * TPp / (p+n) + TNn / (p + n)
     * N((p*tpr(a) + n*tnr(a))/(p+n)
     */
    // The accuracy.
    template <typename T>
    auto accuracy(approximate_set<T> const & a, double p, double n)
    {
        auto const & u = p + n;
        auto mu = (p*tpr(a) + n*tnr(a)) / u;
        auto var = (p*tpr(a)*(1.-tpr(a)) + (n*fpr(a)*(1.-fpr(a)))) / (u*u)
        return alex::stats::normal<double>(mu, var);
    };
        
    // The positive predictive value.
    template <typename T>
    auto ppv(approximate_set<T> const & a, real_interval lambda, double u)
    {
        return pow(1. + tp(a,lambda*u) / fp(a,(1.-lambda)*u),-1.);
    };

    // The negative predictive value.
    template <typename T>
    auto npv(approximate_set<T> const & a, real_interval lambda, double u)
    {
    };
}