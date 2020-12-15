#pragma once

/**
 * Binary classification performance measures for Bernoulli Booleans.
 */

template <size_t N>
auto ppv(random_bernoulli<N,bool> x, double p = 0.5)
{
    return p * tpr(x) / (p * tpr(x) + (1-p) fnr(x));
    // in first-order model with error rate r, p = 0.5 => ppv(x,p) == r.
}

auto accuracy(random_bernoulli<N,bool> x, double p = 0.5)
{

}

template <size_t N>
auto npv(random_bernoulli<N,bool> x, double p = 0.5)
{
    // in first-order model with error rate r, p = 0.5 => npv(x,p) == r.
}

