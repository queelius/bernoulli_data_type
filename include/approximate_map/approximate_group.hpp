#pragma once

#include <optional>

namespace alex::approximate_map
{
/**
 * A group is a pair (G,f) such that
 *     - f is closed: f : G -> G -> G
 *     - identity element: there exists a unique element e such that f(e,x)=f(x,e)=x
 *     - invertible: for each x in G, there exists an x' such that f(x,x')=f(x',x)=e.
 */
template <
    typename F,
    typename G>
class approximate_group
{
public:
    using domain = std::pair<G,G>
    using codomain = G;

    approximate_group(F f) : f_(f) {};

    auto fpr() const
    {
        auto r1 = _f.fpr() * _g.tpr();
        auto r2 = _f.tpr() * _g.fpr();
        auto r3 = _f.fpr() * _g.fpr();
        std::cout << r1 << "\n" << r2 << "\n" << r3 << "\n";
        
        return std::make_pair(std::min(r1, std::min(r2, r3)));
    }

    auto tpr() const
    {
        return f_.tpr() * g_.tpr();  
    }

    X operator()(X const & x, X const & y) const
    {
        return f_(x,y);
    }

private:
    F f_;
};


/**
 * 
 * f : G -> G -> G may be approximated in two essential ways.
 * 
 * G may be replaced by G*, in which case an element x* sampled from G*
 * may have an error according to its approximate value model.
 * 
 * A simple one is given by
 *     P[x* = x] = tpr
 *     sum a in G P[X* = a] = 1-tpr.
 * 
 * A priori, f(x*,y*) is a random variable over G.
 * 
 *     P[f(x*,y*) = f(x,y)] is some probability that is a function of 
 *     the approximation model and the nature of f.
 * 
 *     For instance, if G has only two elements and f is defined by the
 *     Caylee table:
 * 
 *         f|a|b
 *         -+-+--
 *         a|a|b
 *         -+-+-
 *         b|b|a
 *
 *     Then, P[a* = a] = tpr(a) and P[a* != a] = 1-tpr(a) and
 *           P[b* = b] = tpr(b) and P[b* != b] = 1-tpr(b).
 * 
 *     Then, P[f(a*,a*)=a] = the probability that a*=a and a*=a (independent)
 *     which is just tpr(a), so tpr(a)^2. However, it can also be correct
 *     if a*=b and a*=b, which is just (1-tpr(a))+(1-tpr(a)). So, together, it's
 *     just P[f(a*,a*)=a] = tpr(a)*tpr(a) + (1-tpr(a))+(1-tpr(a)).
 *     Thus, it obtains a max at tpr(a) in {0,1} and a min at tpr(a) = 0.5.
 * 
 *     P[f(a*,b*)=b] = all the ways f(x,y)=b, so a*=a & b*=b, a*=b & b*=a,
 *     which is tpr(a)tpr(b) + (1-tpr(a))(1-tpr(b)).
 * 
 *     P[f(b*,a*)=b] = all the ways f(x,y)=b, so again
 *     tpr(a)tpr(b) + (1-tpr(a))(1-tpr(b)). Since f is abelian, order does
 *     not matter, f(a*,b*)=f(b*,a*).
 * 
 *     If f is replaced by f*, then we have a different sort of problem.
 *     Now, f*(a,b), where a,b are degenerate (P[a*=a]=1), maps to
 *     an approximate value. So, f* : {a,b} -> {a*,b*}.
 * 
 *     If f* is a first-order approximation, then in the case above,
 *     f*(a,a) may not equal f(a,a)=a. Generally, it doesn't equal 'a'
 *     due to *rate distortion*. It tries to map it to a, but may fail with
 *     some probability. f* is a rate distortion, whereas G* = {a*,b*} is
 *     typically seen as *noise* even though the noise may be the result
 *     of a rate distrotion from a rate-distorted map.
 *  
 *
 * 
 *         &|0|1
 *         -+-+--
 *         0|0|0
 *         -+-+-
 *         1|0|1
 * 
 *     P[&(0*,0*)=0] = all the ways f(x,y)=0, or alterantively, the complement of
 *     the only way it doesn't equal 0... so P[&(0*,0*)=0]=1-P[0*=1]P[0*=1]=
 *     1-fpr^2(0). P[&(1*,1*)=1] = tpr^2(1).
 *     
 * 
 * 
 * is To minimize error, given the assumption that
 *     the inputs are uniformly distributed, is to make it such that a priori
 *     the most likely output is the least likely to result in an error for
 *     any inputs. Extend this to logical conclusion.
 * 
 * Alternatively, f may be replaced by f* 
 */
