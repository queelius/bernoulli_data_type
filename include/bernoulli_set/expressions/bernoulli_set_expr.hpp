
// what is the probability that a particular relation R <= A x B
// is a subset of A* x B* also? suppose A x B is a subset of U x U.
// then, A* and B* are also subsets of U.
//
// given a relation R in A x B, an approximate relation R* in A x B
// is just an approximate set where the members are pairs.


template <
    typename T,
    template <typename> typename L,
    template <typename> typename R,
    typename I = interval
>
class Intersection
{
public:
    using value_type = T;
    using interval_type = I;

    Intersection(L const & a, R const & b) : a(a), b(b) {};

    bool contains(T const & x)
    {
        return a.contains(x) && b.contains(x);
    };

    I fpr() const
    {
        return one() - (one() - a.fpr()) * (one() - b.fpr());
    };

    I fnr() const
    {
        return span(
            a.fnr() * b.fnr(),
            span(
                a.fnr()*(one()-b.fpr()),
                b.fnr()*(one()-a.fpr())));
    };

    bool operator==(Intersection<T,L,R,I> const & other)
    {
        return a == other.a && b == other.b;
    };

private:
    L<T> a;
    R<T> b;    
};


template <
    typename L,
    typename R,
    typename I = interval
>
class DisjointUnion
{
public:
    using left_value_type = typename L::value_type;
    using right_value_type = typename R::value_type;
    using value_type = std::variant<
        left_value_type,
        right_value_type>;

    DisjointUnion(L a, R b) : a(a), b(b) {};

    bool contains(value_type const & x)
    {
        return x.index() == 0 ?
            a.contains(x) : b.contains(x);
    };

    I fpr() const
    {
        return span(left.fpr(), right.fpr());
    };

    I fnr() const
    {
        return span(left.fnr(), right.fnr());;
    };

    bool operator==(DisjointUnion<L,R,I> const & other)
    {
        return left == other.a && b == other.b;
    };

    auto cardinality() const
    {
        return cardinality(left) + cardinality(right);
    };

private:
    L left;
    R right;    
};


template <typename ASet>
class PowerSet
{
public:
    PowerSet(ASet a) : a(a) {};

    using interval_type = typename ASet::interval_type;
    using value_type = std::set<typename ASet::value_type>;

    bool contains(value_type const & s) const
    {
        for (const auto& x : s)
            if (!a.contains(x)) return false;
        return true;
    };

    bool operator==(const PowerSet<ASet>& ps)
    {
        return a == ps.a;
    };

    auto cardinality() const
    {
        return std::pow(2., a.cardinality());
    };

    interval_type fpr() const
    {
        return {0,0};
    };

    interval_type fnr() const
    {
        return {0,0};
    };

private:
    ASet a;
};

template <
    typename L,
    typename R
>
class CartesianProduct
{
public:
    CartesianProduct(L const & a, R const & b) : a(a), b(b) {};

    using left_value_type = typename Left::value_type;
    using right_value_type = typename Right::value_type;
    using value_type = std::pair<left_value_type, right_value_type>;
    using interval_type = typename Left::interval_type;

    bool contains(value_type const & p) const
    {
        return a.contains(p.first) && b.contains(p.second);
    };

    interval_type fpr() const
    {
        return 0;
    };

    interval_type tpr() const
    {
        return 1;
    };

    bool operator==(const CartesianProduct<L,R>& other)
    {
        return a == other.a && b = other.b;
    };

    auto cardinality() const
    {
        return cardinality(a) * cardinality(b);
    };

    auto left_project() const
    {
        return p.left;
    };

    auto right_project() const
    {
        return p.right;
    };

    auto converse() const
    {
        return CartesianProduct<R,L>(b, a);
    };

private:
    L a;
    R b;
};



/*
    // A probability_type is a probability and
    // thus should take on a value between 0 and 1.
    typedef double probability_type;
    
    // The cardinality_type is able to represent
    // the cardinality of sets. Since an approximate
    // set may have an uncertain number of false
    // positives or true positives, the
    // cardinality_type is a real number type.
    typedef double cardinality_type;
    
    // The value_type is the type of elements
    // in the set.
    typedef X value_type;
    
    // If x is in the approximated set S, returns
    // true with probability
    //     1 - false_negative_rate().
    // Otherwise, returns true with probability
    //     false_positive_rate().
    virtual
    bool contains(const X& x) const = 0;
    
    // The cardinality of the approximate set.
    // If the approximate set has an uncertain number
    // of false positives or true positives, the
    // expected cardinality may be returned.
    virtual
    cardinality_type cardinality() const = 0;
    
    // Returns the false positive rate,
    // the probability that an element
    // not in the approximated set S
    // is in the approximate set S*.
    virtual
    probability_type false_positive_rate() const = 0;

    // Returns the false negative rate,
    // the probability that an element
    // in the approximated set S
    // is not in the approximate set S*.
    virtual
    probability_type false_negative_rate() const = 0;
*/
};
