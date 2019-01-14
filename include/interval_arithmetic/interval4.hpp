#include <algorithm>
#include <stdexcept>

namespace alex
{
    struct LB_0 { static constexpr double get() { return 0; }; };
    struct UB_1 { static constexpr double get() { return 1; }; };

    template <typename T, class Range>
    struct project_policy
    {
        static constexpr T set(const T& x)
        {
            return std::min(Range::upper(), std::max(Range::lower(), x));
        }
    };

    template <typename T, class Range>
    struct no_out_of_range_policy
    {
        static constexpr T set(const T& x) { return x; };
    };

    template <typename T, class Range>
    struct throw_exception_out_of_range_policy
    {
        static constexpr T set(const T& x)
        {
            if (x < Range::lower())
                throw std::out_of_range("throw_exception_out_of_range_policy: value is too small");
            if (x > Range::upper())
                throw std::out_of_range("throw_exception_out_of_range_policy: value is too large");
            return x;
        };
    };

    template <typename T>
    struct real_range
    {
        static constexpr T lower() { return -std::numeric_limits<T>::infinity(); };
        static constexpr T upper() { return std::numeric_limits<T>::infinity(); };
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
        class OutOfRangePolicy = project_policy<T, Range>>
    class interval
    {
    public:
        interval(const T& l, const T& u) :
            _l(l), _u(u) {};

        template <class T2, class OperatorSet2, class Range2, class OutOfRangePolicy2>
        interval(const interval<T2, OperatorSet2, Range2, OutOfRangePolicy2>& copy) :
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