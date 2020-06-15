#pragma once

#include <optional>

namespace alex::approximate_map
{
/**
 * An important operation on maps (functions) is composition.
 * 
 * F after G, i.e., (F . G)(x) = F(G(x))
 */
template <
    typename F,
    typename X,
    typename Y>
class commutative_approximate_partial_binary_map
{
public:
    using domain = std::pair<X,X>
    using codomain = Y;
    //using fpr_interval = decltype(std::declval<F>().fpr());

    // f models a partial binary function f : (X,X) -> Y with the constraint that
    // it is commutative, i.e, f => f(x,y) = f(y,x).
    // it enforces this constraint by evaluating the pair of inputs, which
    // should be totally ordered
    
    commutative_approximate_map(F f) : f_(f) {};

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

    std::optional<Y> operator()(X const & x, X const & y) const
    {
        return x < y ? f_(x,y) : f_(y,x);
    }

private:
    F f_;
};

