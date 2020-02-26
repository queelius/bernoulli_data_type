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
    typename G
>
class approximate_map_composition
{
public:
    using domain = typename G::domain;
    using codomain = typename F::codomain;
    //using fpr_interval = decltype(std::declval<F>().fpr());

    approximate_map_composition(F f, G g) : _f(f), _g(g) {};

    // approximate_map_compose : (Y -> Z, X -> Y) -> (X -> Z)
    // so, codomain of F must be domain of G
    // since random approximate maps are partial functions, even
    // if codomain = domain, the domain of definition of G
    // may not be defined for some x in X or the domain of
    // definition of F may not be defined for y = G(x) in Y.
    // In either case, a 'nothing' value must be returned.
    // We do not use the maybe monad since it is simple enough
    // to encapsulate the behavior in Compose.

    auto fpr() const
    {
        auto r1 = _f.fpr() * _g.tpr();
        auto r2 = _f.tpr() * _g.fpr();
        auto r3 = _f.fpr() * _g.fpr();
        std::cout << r1 << "\n" << r2 << "\n" << r3 << "\n";
        
        return std::make_pair(std::min(r1, std::min(r2, r3)));
    };

    auto tpr() const
    {
        return _f.tpr() * _g.tpr();  
    };

    //auto fnr() const
   // {
    //    return decltype(tpr())(1) - tpr();
    //};

    auto operator()(domain const & x) const
    {
        if (auto y = _g(x) ; y)
            return _f(*y);
        else
            return std::optional<codomain>{};
    };

private:
    F _f;
    G _g;
};

template <typename F, typename G>
auto approximate_map_compose(F f, G g)
{
    return approximate_map_composition<F,G>(f,g);
};

template <typename X>
class identity
{
public:
    using domain = X;
    using codomain = X;

    double fpr() const { return 0; };
    double tpr() const { return 1; };

    auto operator()(domain const & x) const { return x; };
};
}