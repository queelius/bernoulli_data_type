template <typename <typename> Set, typename X, typename Interval>
class ASetIterableUnion: public ASetUnion<X, Interval>
{
public:
    using typename ASet<X, Interval>::value_type;
    using typename ASet<X, Interval>::interval_type;
    
    ASetIterableUnion(ASetIterable<X, Interval> const * const s1,
              ASetIterable<X, Interval> const * const s2)
              : ASet<X, Interval>(s1, s2),
                _s(std::begin(*s1), std::end(*s1))
    {
        _s.insert(std::begin(*s2), std::end(*s2));
    };
    
    cardinality_type cardinality() const
    {
        return static_cast<cardinality_type>(_s.size());
    };
    
    bool contains(const value_type& x) const
    {
        return _s.count(x) == 1;
    };
    
    auto begin() const
    {
        return std::begin(_s);
    };
    
    auto end() const
    {
        return std::end(_s);
    };

private:
    Set<X> _s;
};

template <typename Set1, typename Set2, typename X, typename Interval>
ASetIterableP<Set1, X, Interval> make_union(
    ASetIterable<Set1, X, Interval> const * const s1,
    ASetIterable<Set2, X, Interval> const * const s2)
{
    return make_unique(
        ASetIterableUnion<Set1, X, Interval>(s1, s2));
};
