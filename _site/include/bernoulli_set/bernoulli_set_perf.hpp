#include <algebraic_random_elements/random_variables/normal.hpp>

namespace bernoulli_set
{
    template <bernoulli_set S>
    normal tp(S const & a, double p = 1)
    {
        return normal(p*a.tpr(), p*a.tpr()*(1-a.tpr()));
    }

    template <bernoulli_set S>
    normal fp(S const & a, double n = 1)
    {
        return normal(n*a.fpr(), n*a.fpr()*tnr(a)));
    }

    template <bernoulli_set S>
    normal tn(S const & a, double n = 1)
    {
        return normal(p*tnr(a), n*tnr(a)*(1-a.fpr()));
    }

    template <bernoulli_set S>
    normal fn(S const & a, double p = 1)
    {
        return normal(p*fnr(a), p*fnr(a)*fnr(a));
    }

    /**
     * The true positive rate. This is the true positive rate
     * as a random variable given an expected true positive rate
     * of a.tpr() and p positives.
     * 
     * If p is unknown, a worst-case estimate is p=1.
     */
    template <bernoulli_set S>
    normal tpr(S const & a, double p = 1)
    {
        return normal(a.tpr(), a.tpr()*(1-a.tpr())/p);
    }

    // The false positive rate.
    template <bernoulli_set S>
    normal fpr(S const & a, double p = 1)
    {
        return normal(a.fpr(), a.fpr()*(1-a.fpr())/n);
    }

    // The false negative rate.
    template <bernoulli_set S>
    normal fnr(S const & a, double p)
    {
        return normal(1.-a.tpr(), a.tpr()*(1-a.tpr())/p);
    }

    // The true negative rate.
    template <bernoulli_set S>
    normal tnr(S const & a, double n)
    {
        auto tnr_ = 1.-a.fpr();
        return normal(1.-a.fpr(),a.fpr()*(1-a.fpr())/n);
    }

    /**
     * TPp / (p+n) + TNn / (p + n)
     * N((p*tpr(a) + n*tnr(a))/(p+n)
     */
    // The accuracy.
    template <bernoulli_set S>
    auto accuracy(S const & a, double p, double n)
    {
        auto u = p + n;
        auto mu = (p*tpr(a) + n*tnr(a)) / u;
        auto var = (p*tpr(a)*(1.-tpr(a)) + (n*fpr(a)*(1.-fpr(a)))) / (u*u)
        return normal<double>(mu,var);
    }
        
    // The positive predictive value.
    template <bernoulli_set S>
    auto ppv(S const & a, double lam, double u)
    {
        // return a random variable
        return std::pow(1. + tp(a,lam*u) / fp(a,(1.-lam)*u),-1.);
    }

    // The negative predictive value.
    template <bernoulli_set S>
    auto npv(S const & a, double lam, double u)
    {
        // return a random variable
        return 0;
    }
}