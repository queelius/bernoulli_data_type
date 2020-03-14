

template <typename T>
concept bool Set = requires(T x)
{
    { x.contains(typename T::value_type) } -> bool;
};

template <typename T>
concept bool Iterable() 
{
    return requires(T x)
    {
        { x.begin() };
        { x.end() };
    } || requires(T x)
    {
        { std::begin(x) };
        { std::end(x) };
    } || requires(T x)
    {
        { begin(x) };
        { end(x) };
    };

};

template <typename T>
concept bool RandomPositiveApproximateSet() 
{
    return requires(T x)
    {
        { false_positive_rate(x); }
    } && Set<T>
};

template <typename T>
concept bool RandomNegativeApproximateSet() 
{
    return requires(T x)
    {
        { false_negative_rate(x); }
    } && Set<T>
};

template <typename T>
concept bool RandomApproximateSet =
    RandomPositiveApproximateSet<T> ||
    RandomNegativeApproximateSet<T>;

template <typename T>
concept bool IterableRandomApproximateSet =
    (RandomPositiveApproximateSet<T> ||
    RandomNegativeApproximateSet<T>) &&
    Iterable<T>;


template <IterableRandomApproximateSet T1,
          IterableRandomApproximateSet T2>
    requires Convertible<typename T2::value_type,typename T1::value_type>
auto make_union(T1 a, T2 b)
{
    using value_type = typename T1::value_type;

    std::vector<value_type> xs(begin(a), end(a));
    xs.insert(std::end(xs), begin(b), end(b));
    return T(begin(xs), std::end(xs), fpr, fnr);
}