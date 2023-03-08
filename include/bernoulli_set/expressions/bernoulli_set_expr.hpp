#pragma once

namespace bernoulli::set::algebra
{
    template <typename E>
    class bernoulli_set_expr
    {
    public:
    	using value_type = typename E::value_type;
    	
        bernoulli_set_expr(E const & e) : e(e) {}
        
        auto error_rate(T const & x) { return static_cast<E>(e).error_rate(x); }
        auto false_positive_rate() const { return static_cast<E>(e).false_positive_rate(); }
        auto false_negative_rate() const { return static_cast<E>(e).false_negative_rate(); }
        auto contains( const & x) const { return static_cast<E>(e).contains(x); }

    private:
        E const & e;
    };
}
