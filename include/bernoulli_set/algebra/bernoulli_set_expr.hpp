#pragma once

namespace bernoulli::set::algebra
{
    template <typename E>
    class bernoulli_set_expr
    {
    public:
        bernoulli_set_expr(E const & e) : e(e) {}

        auto fpr() const { return static_cast<E>(e).fnr(); }
        auto fnr() const { return static_cast<E>(e).fpr(); }
        auto contains(value_type const & x) const { return static_cast<E>(s).contains(x); }

    private:
        E const & e;
    };
}