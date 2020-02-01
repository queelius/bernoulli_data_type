#include <set>
#include <utility>
#include "binary_relation_expr.hpp"

using std::set;
using std::pair;
using std::make_pair;
using std::move;

namespace alex::relation
{
    template <
        typename T,
        typename U,
        template <typename> typename Container = set>
    class mutable_binary_relation:
        public binary_relation_expr<mutable_binary_relation<T,U,Container>,T,U>
    {
    public:
        using value_type = binary_relation_expr<mutable_binary_relation<T,U,Container>,T,U>::value_type;
        using container = Container<value_type>;
        using size_type = container::size_type;
        using iterator = container::iterator;
        using const_iterator = container::const_iterator;

        iterator begin() { return c_.begin(); }
        iterator end() { return c_.end(); }
        const_iterator cbegin() const { return c_.cbegin(); }
        const_iterator cend() const { return c_.cend(); }

        mutable_binary_relation(mutable_binary_relation &&) = default;
        mutable_binary_relation(mutable_binary_relation const &) = default;
        mutable_binary_relation() = default;
        virtual ~mutable_binary_relation() {};

        auto size() const { return c_.size(); };
        constexpr auto dim() const { return 2; };

        auto operator()(value_type const & x) const
        {
            return c_.count(x) != 0;
        };

        auto operator()(T x, U y) const
        {
            return operator()(make_pair(move(x),move(y)));
        };

        auto empty() const
        {
            return c_.empty();
        };

        auto add(value_type x)
        {
            return c_.insert(move(x));
        };

        auto add(T x, U y)
        {
            return c_.emplace(move(x),move(y));
        };

        auto remove(T x, U y)
        {
            return c_.erase(make_pair(move(x),move(y)));
        };

        auto remove(value_type x)
        {
            return c_.erase(move(x));
        };

        auto image(T const & x) const
        {
            set<U> ys;
            for (auto const & [v,w] : c_)
                if (v == x) ys.insert(w);
            return ys;
        };

        auto preimage(U const & y) const
        {
            set<T> xs;
            for (auto const & [v,w] : c_)
                if (v == y) xs.insert(w);
            return xs;
        };

        auto const & data() const { return c_; };

        template <typename E, typename X, typename Y>
        mutable_binary_relation(binary_relation_expr<E,T,U> const & r, X begin1, X end1, Y begin2, Y end2)
        {
            for (auto i = begin1; i != end1; ++i)
                for (auto j = begin2; j != end2; ++j)
                    if (r(*i,*j)) c_.add(*i,*j);
        };

        template <typename E, typename XY>
        mutable_binary_relation(binary_relation_expr<E,T,U> const & r, XY begin, XY end)
        {
            for (auto i = begin; i != end; ++i)
                if (r(*i)) c_.add(*i);
        };

        // conceptually, we start with a relation, a set of tuples. we apply
        // algebraic operations, like union, compose, or complement, which
        // generates new relations.
        //
        // when we do a complement, we can no longer iterate over the members
        // unless we have a universal set (relation) that may be referenced.
        //
        // this makes constructing relations and placing them into the
        // container problematic. 
        template <typename E>
        mutable_binary_relation(binary_relation_expr<E,T,U> const & r) : c_(r.cbegin(), r.cend()) {};

    private:
        container c_;
    };
}