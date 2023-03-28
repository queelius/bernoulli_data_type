#pragma once

#include "bernoulli_set_expr.hpp"

template <
    typename L,
    typename R
>
class bernoulli_disjoint_union_set_expr:
    public bernoulli_set_expr<bernoulli_disjoint_union_set_expr<L,R>>
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::variant<
        left_value_type,
        right_value_type>;

    bernoulli_disjoint_union_set_expr(
        L const & left,
        R const & right) : left_(left), right_(right) {};

    auto left() const { return left; }
    auto right() const { return right; }

    auto contains(left_value_type const & x) { return left.contains(x); }
    auto contains(right_value_type const & x) { return right.contains(x); }

private:
    L const & left_;
    R const & right_;    
};

template <
    typename L,
    typename R
>
auto
make_disjoint_union(L left, R right)
{
    return bernoulli_disjoint_union_set_expr<L,R>(left,right);
}
