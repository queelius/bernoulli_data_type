#pragma once

#include <algorithm>
#include <limits>
#include <iterator>
#include <type_traits>
#include <limits>

namespace alex::math
{
    // T models a number type
	template <typename T>
	class interval
	{
	public:
		using value_type = T;

		interval() : _r{ T(1),T(0) } {};
		interval(const T& x) : _r{ x,x } {};
		interval(const T& x1, const T& x2) : _r{ std::min(x1, x2), std::max(x1, x2) } {};

        bool is_empty() const { return max() < min(); };
		bool is_degenerate() const { return min() == max(); };
		T min() const { return _r[0]; };
		T max() const { return _r[1]; };

	private:
		T _r[2];
	};

	template<typename T>
	struct is_ranged
	{
	private:
		template<typename U> static auto test(int) ->
			decltype(std::declval<U>().min() == 1 && std::declval<U>().max() == 1, std::true_type());

		template<typename> static std::false_type test(...);

	public:
		static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value;
	};

    using real_interval = interval<double>;

    template <typename T1, typename T2>
    auto make_interval(const T1& x, const T2& y)
    {
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

	template <typename T1, typename T2>
	auto operator*(const T1& lhs, const interval<T2>& rhs)
	{
		return interval<T1>(lhs) * rhs;
	};

	template <typename T1, typename T2>
	auto operator*(const interval<T1>& lhs, const T2& rhs)
	{
		return lhs * interval<T2>(rhs);
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
		return make_interval<T>(-rhs.max(), -rhs.min());
	};

	template <typename T>
	interval<T> operator+(const interval<T>& rhs)
	{
		return rhs;
	};

	template <typename T1, typename T2>
	auto operator*(const interval<T1>& lhs, const interval<T2>& rhs)
	{
		return make_interval(
			std::min(std::min(std::min(lhs.min()*rhs.min(), lhs.min()*rhs.max()),
				lhs.max()*rhs.min()), lhs.max()*rhs.max()),
			std::max(std::max(std::max(lhs.min()*rhs.min(), lhs.min()*rhs.max()),
				lhs.max()*rhs.min()), lhs.max()*rhs.max()));
	};

	template <typename InputIterator>
	auto min_span(InputIterator begin, InputIterator end)
	{
		using base_type = typename std::iterator_traits<InputIterator>::value_type;

		if constexpr(is_ranged<base_type>::value)
		{
			using T = typename base_type::value_type;
			auto lo = std::numeric_limits<T>::infinity();
			auto hi = -lo;
			for (auto i = begin; i != end; ++i)
			{
				lo = std::min(lo, i->min());
				hi = std::max(hi, i->max());
			}
			return base_type(lo, hi);
		}
		else
		{
			auto lo = std::numeric_limits<base_type>::infinity();
			auto hi = -lo;
			for (auto i = begin; i != end; ++i)
			{
				lo = std::min(lo, *i);
				hi = std::max(hi, *i);
			}
			return make_interval(lo, hi);
		}
	};

	template <typename Collection>
	auto min_span(Collection intervals)
	{
		return min_span(std::begin(intervals), std::end(intervals));
	};

	template <typename T>
	auto min_span(std::initializer_list<T> intervals)
	{
		return min_span(std::begin(intervals), std::end(intervals));
	};

	real_interval one()
	{
		return real_interval(1,1);
	};

	real_interval zero()
	{
		return real_interval(0,0);
	};
}