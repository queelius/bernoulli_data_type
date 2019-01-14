#include <algorithm>
#include <stdexcept>

namespace alex
{
    template <typename T>
    struct zero { static constexpr T get() { return 0; }; };

    template <typename T>
    struct unity { static constexpr T get() { return 1; }; };

    template <typename T>
    struct neg_inf { static constexpr T get() { return -std::numeric_limits<T>::infinity(); }; };

    template <typename T>
    struct pos_inf { static constexpr T get() { return std::numeric_limits<T>::infinity(); }; };

    template <typename T,
        template <class> class LB,
        template <class> class UB>
    struct range
    {
        static constexpr T lower() { return typename LB<T>::get(); };
        static constexpr T upper() { return typename UB<T>::get(); };
    };

    template <typename T>
    using real_range = range<T, neg_inf, pos_inf>;

    template <typename T>
    using unit_range = range<T, zero, unity>;

    template <typename T, class Range>
    struct project_assignment_policy
    {
        static T set(const T& x)
        {
            return std::min(typename Range::upper(), std::max(typename Range::lower(), x));
        }
    };

    template <typename T, class Range>
    struct default_assignment_policy
    {
        static constexpr T set(const T& x) { return x; };
    };


    struct set_theoretic_operator_set
    {
        template <typename X, typename Y>
        static X mul(const X& lhs, const Y& rhs)
        {
            return X{ std::max(lhs.min(), rhs.min()), std::min(lhs.max(), rhs.max()) };
        };
    };

    struct interval_arithmetic_operator_set
    {
        template <typename X, typename Y>
        static X add(const X& lhs, const Y& rhs)
        {
            return X{ lhs.min() + rhs.min(), lhs.max() + rhs.max() };
        };
    };

    template <
        class T = double,
        class OperatorSet = interval_arithmetic_operator_set,
        class Range = real_range<T>,
        class AssignmentPolicy = default_assignment_policy<T, Range>>
    class interval
    {
    public:
        interval(const T& l, const T& u) :
            _l(l), _u(u) {};

        template <class T2, class OperatorSet2, class Range2, class AssignmentPolicy2>
        interval(const interval<T2, OperatorSet2, Range2, AssignmentPolicy2>& copy) :
            _l(copy.min()), _u(copy.max()) {};

        bool empty() const { return _u < _l; };
        T min() const { return _l; };
        T max() const { return _u; };
        T& min() { return _l; };
        T& max() { return _u; };

    private:
        T _l, _u;
    };

    template <
        class T1,
        class T2,
        class OperatorSet,
        template <class> class Range>
    auto operator+(
        const interval<T1, OperatorSet>& lhs,
        const interval<T2, OperatorSet>& rhs)
    {
        return OperatorSet::add(lhs, rhs);
    }

    template <
        class T1,
        class T2,
        class OperatorSet>
    auto operator*(
        const interval<T1, OperatorSet>& lhs,
        const interval<T2, OperatorSet>& rhs)
    {
        return OperatorSet::mul(lhs, rhs);
    }

}