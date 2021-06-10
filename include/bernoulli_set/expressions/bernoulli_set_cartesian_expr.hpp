#pragma once

#include "bernoulli_set_expr.hpp"

namespace bernoulli::set::expression
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
            : left(left), right(right) {};

        using left_value_type = typename L::value_type;
        using right_value_type = typename R::value_type;
        using value_type = std::pair<left_value_type,right_value_type>;

        auto contains(value_type const & p) const
        {
            return a.contains(p.first) && b.contains(p.second);
        }

        auto fpr() const
        {
            return 0;
        }

        auto tpr() const
        {
            return 1;
        }

        double cardinality() const
        {
            return cardinality(left) * cardinality(right);
        }

        auto left_project() const
        {
            return left;
        }

        auto right_project() const
        {
            return right;
        }

        auto flip() const
        {
            return bernoulli_set_cartesian_prod_expr<R,L>(right,left);
        }

        L const & left;
        R const & right;
    };

    template <typename L, typename R>
    auto operator==(
        bernoulli_set_cartesian_prod_expr<L,R> const & lhs,
        bernoulli_set_cartesian_prod_expr<L,R> const & rhs)
    {
        return lhs.left == rhs.left && lhs.right == rhs.right;
    }

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