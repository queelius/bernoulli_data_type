#pragma once

#include "bernoulli_set_expr.hpp"

namespace bernoulli::set::algebra
{
    template <
        typename L,
        typename R
    >
    class bernoulli_set_cartesian_prod_expr:
        public bernoulli_set_expr<bernoulli_set_cartesian_prod_expr<L,R>>
    {
    public:
        bernoulli_set_cartesian_prod_expr(L const & left, R const & right)
            : left_(left), right_(right) {};

        using left_value_type = typename L::value_type;
        using right_value_type = typename R::value_type;
        using value_type = std::pair<left_value_type,right_value_type>;

        /**
         * Note that `left.contains` and `right.contains` are typically
         * Bernoulli Booleans, which implement `&&`, i.e.,
         * ```
         *     && : (bernoulli_bool<N1>, bernoulli_bool<N2> -> bernoulli_bool<N3>
         * ```
         * and thus `contains` returns a Bernoulli Boolean that models
         * `bernoulli_bool<N2>`.
         */
        auto contains(value_type const & p) const
        {
            return left_.contains(p.first) && right_.contains(p.second);
        }

        auto fpr() const
        {
            throw "not implemented yet";
        }

        auto tpr() const
        {
            throw "not implemented yet";
        }

        auto left() const { return left_; }
        auto right() const { return right_; }

        auto flip() const { return bernoulli_set_cartesian_prod_expr<R,L>(right_,left_) }

        L const & left_;
        R const & right_;
    };

    template <
        typename L,
        typename R
    >
    auto
    make_cartesian_product(L const & left, R const & right)
    {
        return bernoulli_set_cartesian_prod_expr<L,R>(left,right);
    }
}