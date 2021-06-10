

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
        R const & right) : left(left), right(right) {};

    auto contains(value_type const & x)
    {

    }

    auto contains(left_value_type const & x)
    {
        return left.contains(x);
    }

    auto contains(right_value_type const & x)
    {
        return right.contains(x);
    }

    auto cardinality() const
    {
        return cardinality(left) + cardinality(right);
    }

private:
    L const & left;
    R const & right;    
};

template <typename L, typename R>
auto operator==(    
    bernoulli_disjoint_union_set_expr<L,R> const & lhs,
    bernoulli_disjoint_union_set_expr<L,R> const & rhs)
{
    return lhs.left == rhs.left && lhs.right == rhs.right;
}


template <
    typename L,
    typename R
>
auto
make_discriminated_union(L left, R right)
{
    return DiscriminatedUnion<L,R>(left,right);
}
