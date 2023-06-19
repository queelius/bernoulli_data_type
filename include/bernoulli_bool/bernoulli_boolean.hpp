#pragma once

/**
 * A realization of a value sampled from a random Bernoulli value
 * of type Boolean.
 */

struct bernoulli_bool
{
    bool value;
    double err;
    
    operator bool() const { return value; }
};

auto operator!(bernoulli_bool a)
{
    return bernoulli_bool{!a.value,a.err};
}

auto operator&&(bernoulli_bool a,
                bernoulli_bool b)
{
    
    return a.value && b.value ?
    	bernoulli_bool{true,a.err+b.err-a.err*b.err} : // prob. either are errors
        bernoulli_bool{false,a.err*b.err}; // prob. both are errors
}

auto operator||(bernoulli_bool a,
                bernoulli_bool b)
{
    return a.value && b.value  ? bernoulli_bool{true,a.err+b.err-a.err*b.err} :
           !a.value && b.value ? bernoulli_bool{true,a.err-a.err*b.err} :
           a.value && !b.value ? bernoulli_bool{true,b.err-a.err*b.err} :
                                 bernoulli_bool{true,a.err*b.err};
}

/**
* Predictive value (pv)
* ---------------------
* Suppose
*     X' ~ bernoulli_bool 
* Then, by the Bernoulli model,
*     P[X' != a|X == a] == error(a).
* 
* In the above, we are given an observation of X' == a with X unknown, but
* we are typically interested in
*     pv(X') := P[X == a|X' == a]
* which, by the law of probability
*     pv(X') = P[X == a,X' == a] / P[X' == a]
*            = P[X' == a|X == a] P[X == a] / P[X' == a]
*            = (1-error(a)) P[X == a] / P[X' == a].
* 
* By the total law of probability,
*     P[X' == a] = P[X' == a,X == a] + P[X' == a,X != a]
*                = P[X' == a|X == a]P[X == a] + P[X' == a|X != a]P[X != a]
*                = (1-error(a))P[X == a] + error(a)(1-P[X == a]).
* 
* If we denote P[X == a] by p(a) and substitute P[X' == a] with the above,
* we get the result
*     pv(a) =          p(a)*(1-error(a))
*                --------------------------------.
*                p(a)*(1-error(a))+(1-p(a))*error(a)
*/

auto pv(bernoulli_bool const & x, double p = 0.5)
{
    return p*(1-x.err) / (p*(1-x.err + (1-p)*x.err));
}


auto nor(bernoulli_bool a,
         bernoulli_bool b)
{
    return ~(a | b);
}

auto nand(bernoulli_bool a,
          bernoulli_bool b)
{
    return ~(a & b);
}

auto operator^(bernoulli_bool a,
               bernoulli_bool b)
{
    return (~a & b) | (a & ~b);
}

