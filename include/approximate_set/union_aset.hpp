#include <algorithm>
#include <utility>
using std::enable_if;
using std::is_same;
using std::declval;
#include <iostream>
#include "interval0_1.hpp"
#include "disjoint_interval_set.hpp"

namespace alex::aset
{
    namespace fnr_union_operation
    {
        using alex::math::interval0_1;
        //using alex::math::disjoint_interval_set;

        template <typename T>
        interval0_1<T> operator|(const interval0_1<T>& lhs, const interval0_1<T>& rhs)
        {
            auto l = lhs.min() < rhs.min() ? lhs.min() : rhs.min();
            auto r = lhs.max() > rhs.max() ? lhs.max() : rhs.max();
            return interval0_1<T>(l, r);
        };

        /*
        template <typename T>
        disjoint_interval_set0_1<T> operator|(const interval0_1<T>& lhs, const interval0_1<T>& rhs)
        {
            disjoint_interval_set0_1<T> result{lhs};
            return lhs.unite(rhs);
        };

        template <typename T>
        disjoint_interval_set0_1<T> operator|(const disjoint_interval_set0_1<T>& lhs, const disjoint_interval_set0_1<T>& rhs)
        {
            return lhs.unite(rhs);
        };
        */
    }

    template <class A, class B,
        typename = typename enable_if<is_same<typename A::value_type, typename B::value_type>::value>::type>
    class union_aset
    {
    public:
        using value_type = typename A::value_type;
        using fpr_type = decltype(declval<A>().fpr());
        using fnr_type = decltype(declval<B>().fnr());

        union_aset(A& a, B& b) : a(a), b(b) {};

        bool contains(const value_type& x) { return a.contains(x) || b.contains(x); };

        auto fpr() const
        {
            return 1 - (1 - a.fpr()) * (1 - b.fpr());
        };

        auto fnr() const
        {
            using namespace fnr_union_operation;
            return (a.fnr()*(1-b.fpr())) | (b.fnr()*(1-a.fpr())) | ((a.fnr() * b.fnr()));
        };

    private:
        A a;
        B b;
    };

    template <class A, class B>
    auto union_of(A a, B b)
    {
        return union_aset<A, B>(a, b);
    };
}