#include <algorithm>
#include <stdexcept>

namespace alex
{
	template<typename T>
	struct is_interval
	{
	private:
		template<typename U> static auto test(int) ->
			decltype(std::declval<U>().min() == 1 && std::declval<U>().max() == 1, std::true_type());

		template<typename> static std::false_type test(...);

	public:
		static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value;
	};

    template <class T>
    class interval
    {
    public:
        interval(const T& l, const T& u) :
            _l(l), _u(u) {};

        bool empty() const { return _u < _l; };
        T min() const { return _l; };
        T max() const { return _u; };
        T& min() { return _l; };
        T& max() { return _u; };

    private:
        T _l, _u;
    };

    template <typename T1, typename T2>
    auto make_interval(const T1& x, const T2& y)
    {
        static_assert(std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value);
        if constexpr(std::is_floating_point<T1>::value)
        {
            if constexpr(std::is_floating_point<T2>::value && sizeof(T1) < sizeof(T2))
                return interval<T2>(x, y);
            else
                return interval<T1>(x, y);
        }
        else
        {
            if constexpr(std::is_floating_point<T2>::value)
                return interval<T2>(x, y);
            else
                return interval<double>(x, y);
        }
    };

    namespace min_span_set_theory_op_set
    {
        // minimum interval which is a superset of lhs and rhs.
        template <typename T1, typename T2>
        auto operator+(
            const interval<T1>& lhs,
            const interval<T2>& rhs)
        {
            return make_interval(std::min(lhs.min(), rhs.min()), std::max(lhs.max(), rhs.max()));
        };

        template <typename T1, typename T2>
        auto operator+(
            const interval<T1>& lhs,
            const T2& rhs)
        {
            return make_interval(std::min(lhs.min(), rhs), std::max(lhs.max(), rhs));
        };

        template <typename T1, typename T2>
        auto operator+(
            const T1& lhs,
            const interval<T2>& rhs)
        {
            return make_interval(std::min(lhs, rhs.min()), std::max(lhs, rhs.max()));
        };

        // intersection
        template <typename T1, typename T2>
        auto operator*(
            const interval<T1>& lhs,
            const interval<T2>& rhs)
        {
            return make_interval(std::max(lhs.min(), rhs.min()), std::min(lhs.max(), rhs.max()));
        };

        template <typename T1, typename T2>
        auto operator*(
            const T1& lhs,
            const interval<T2>& rhs)
        {
            return make_interval(std::max(lhs, rhs.min()), std::min(lhs, rhs.max()));
        };

        template <typename T1, typename T2>
        auto operator*(
            const interval<T1>& lhs,
            const T2& rhs)
        {
            return make_interval(std::max(lhs.min(), rhs), std::min(lhs.max(), rhs));
        };
    }

    namespace interval_arithmetic_op_set
    {
        template <typename T1, typename T2>
        auto operator*(const interval<T1>& lhs, const interval<T2>& rhs)
        {
            return make_interval(
                std::min(std::min(std::min(lhs.min()*rhs.min(), lhs.min()*rhs.max()),
                    lhs.max()*rhs.min()), lhs.max()*rhs.max()),
                std::max(std::max(std::max(lhs.min()*rhs.min(), lhs.min()*rhs.max()),
                    lhs.max()*rhs.min()), lhs.max()*rhs.max()));
        };

        template <typename T1, typename T2>
        auto operator*(const T1& lhs, const interval<T2>& rhs)
        {
            return make_interval(
                std::min(lhs*rhs.min(), lhs*rhs.max()),
                std::max(lhs*rhs.min(), lhs*rhs.max()));
        };

        template <typename T1, typename T2>
        auto operator*(const interval<T1>& lhs, const T2& rhs)
        {
            return make_interval(
                std::min(lhs.min()*rhs, lhs.max()*rhs),
                std::max(lhs.min()*rhs, lhs.max()*rhs));
        };

        template <typename T1, typename T2>
        auto operator+(const interval<T1>& lhs, const interval<T2>& rhs)
        {
            return make_interval(lhs.min() + rhs.min(), lhs.max() + rhs.max());
        };

        template <typename T1, typename T2>
        auto operator-(const interval<T1>& lhs, const interval<T2>& rhs)
        {
            return make_interval(lhs.min() - rhs.max(), lhs.max() - rhs.min());
        };

        template <typename T1, typename T2>
        auto operator-(const T1& lhs, const interval<T2>& rhs)
        {
            return make_interval(lhs - rhs.max(), lhs - rhs.min());
        };

        template <typename T1, typename T2>
        auto operator-(const interval<T1>& lhs, const T2& rhs)
        {
            return make_interval(lhs.min() - rhs, lhs.max() - rhs);
        };

        template <typename T1, typename T2>
        auto operator+(const T1& lhs, const interval<T2>& rhs)
        {
            return make_interval(lhs + rhs.min(), lhs + rhs.max());
        };

        template <typename T1, typename T2>
        auto operator+(const interval<T1>& lhs, const T2& rhs)
        {
            return make_interval(lhs.min() + lhs, lhs.max() + rhs);
        };

        template <typename T>
        auto operator-(const interval<T>& rhs)
        {
            return make_interval(-rhs.max(), -rhs.min());
        };
    }
}