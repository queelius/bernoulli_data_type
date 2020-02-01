#include <set>
#include <utility>
//#include "unary_relation_expr.hpp"

using std::set;
using std::move;

namespace alex::relation
{
    template <
        typename T,
        template <typename> typename Container = set>
    class mutable_unary_relation:
        //public unary_relation_expr<mutable_unary_relation<T,Container>,T>
    {
    public:
        //using value_type = unary_relation_expr<mutable_unary_relation<T,Container>,T>::value_type;
        using container = Container<T>;
        using size_type = container::size_type;
        using iterator = container::iterator;
        using const_iterator = container::const_iterator;

        iterator begin() { return c_.begin(); }
        iterator end() { return c_.end(); }
        const_iterator cbegin() const { return c_.cbegin(); }
        const_iterator cend() const { return c_.cend(); }

        mutable_unary_relation(mutable_unary_relation &&) = default;
        mutable_unary_relation(mutable_unary_relation const &) = default;
        mutable_unary_relation() = default;
        virtual ~mutable_unary_relation() {};

        auto size() const { return c_.size(); };
        constexpr auto arity() const { return 1; };

        auto operator()(T t) const
        {
            return operator()(move(t));
        };

        auto empty() const
        {
            return c_.empty();
        };

        auto add(T x)
        {
            return c_.insert(move(x));
        };

        auto remove(T x)
        {
            return c_.erase(move(x));
        };

        container const & data() const { return c_; };

        template <typename E, typename X, typename Y>
        mutable_unary_relation(unary_relation_expr<E,T> const & r, X begin, X end)
        {
            for (auto i = begin; i != end; ++i)
                if (r(*i)) c_.add(*i);
        };

    private:
        container c_;
    };
}