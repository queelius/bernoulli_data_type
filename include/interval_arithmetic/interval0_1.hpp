#ifndef __INTERVAL_H__
#define __INTERVAL_H__

#include <algorithm>
#include <limits>
#include <iterator>
#include <type_traits>
#include "disjoint_interval_set.hpp"

using std::is_same;
using std::true_type;
using std::false_type;
using std::declval;

namespace alex::math
{
	template<typename T>
	struct is_ranged
	{
	private:
		template<typename U> static auto test(int) ->
			decltype(declval<U>().min() == 1 && declval<U>().max() == 1, true_type());

		template<typename> static false_type test(...);

	public:
		static constexpr bool value = is_same<decltype(test<T>(0)), true_type>::value;
	};

	// T is a number type
	template <typename T = double>
	class interval0_1
	{
	public:
		using value_type = T;

		interval0_1() : _r{ T(0),T(1) } {};
		interval0_1(const T& x) : _r{ std::max(T(0),x), std::min(T(1),x) } {};
		interval0_1(const T& x1, const T& x2) : _r{ std::max(T(0), std::min(x1, x2)), std::min(T(1),std::max(x1, x2)) } {};
		bool is_degenerate() const { return min() == max(); }
		T min() const { return _r[0]; };
		T max() const { return _r[1]; };
		T& min() { return _r[0]; };
		T& max() { return _r[1]; };

	private:
		double _r[2];
	};

    template <typename T1, typename T2>
    auto make_interval0_1(const T1& x, const T2& y)
    {
        if constexpr(std::is_floating_point<T1>::value)
        {
            if constexpr(std::is_floating_point<T2>::value && sizeof(T1) < sizeof(T2))
                return interval0_1<T2>(x, y);
            else
                return interval0_1<T1>(x, y);
        }
        else
        {
            if constexpr(std::is_floating_point<T2>::value)
                return interval0_1<T2>(x, y);
            else
                return interval0_1<double>(x, y);
        }
    };

	template <typename T1, typename T2>
	auto operator*(const T1& lhs, const interval0_1<T2>& rhs)
	{
		return interval0_1<T1>(lhs) * rhs;
	};

	template <typename T1, typename T2>
	auto operator*(const interval0_1<T1>& lhs, const T2& rhs)
	{
		return lhs * interval0_1<T2>(rhs);
	};

	template <typename T1, typename T2>
	auto operator+(const interval0_1<T1>& lhs, const interval0_1<T2>& rhs)
	{
		return make_interval0_1(lhs.min() + rhs.min(), lhs.max() + rhs.max());
	};

	template <typename T1, typename T2>
	auto operator-(const interval0_1<T1>& lhs, const interval0_1<T2>& rhs)
	{
		return make_interval0_1(lhs.min() - rhs.max(), lhs.max() - rhs.min());
	};

	template <typename T1, typename T2>
	auto operator-(const T1& lhs, const interval0_1<T2>& rhs)
	{
		return make_interval0_1(lhs - rhs.max(), lhs - rhs.min());
	};

	template <typename T1, typename T2>
	auto operator-(const interval0_1<T1>& lhs, const T2& rhs)
	{
        return make_interval0_1(lhs.min() - rhs, lhs.max() - rhs);
	};

	template <typename T1, typename T2>
	auto operator+(const T1& lhs, const interval0_1<T2>& rhs)
	{
		return make_interval0_1(lhs + rhs.min(), lhs + rhs.max());
	};

	template <typename T1, typename T2>
	auto operator+(const interval0_1<T1>& lhs, const T2& rhs)
	{
		return make_interval0_1(lhs.min() + lhs, lhs.max() + rhs);
	};

	template <typename T>
	auto operator-(const interval0_1<T>& rhs)
	{
		return make_interval0_1(-rhs.max(), -rhs.min());
	};

	template <typename T>
	auto operator+(const interval0_1<T>& rhs)
	{
		return rhs;
	};

	template <typename T1, typename T2>
	auto operator*(const interval0_1<T1>& lhs, const interval0_1<T2>& rhs)
	{
		return make_interval0_1(
			std::min(std::min(std::min(lhs.min()*rhs.min(), lhs.min()*rhs.max()),
				lhs.max()*rhs.min()), lhs.max()*rhs.max()),
			std::max(std::max(std::max(lhs.min()*rhs.min(), lhs.min()*rhs.max()),
				lhs.max()*rhs.min()), lhs.max()*rhs.max()));
	};

    template <typename T>
    class disjoint_interval_set0_1
    {
    public:
        using value_type = typename interval0_1<T>::value_type;
		using const_iterator = typename std::vector<interval0_1<T>>::const_iterator;

        disjoint_interval_set0_1() {};
        disjoint_interval_set0_1(const disjoint_interval_set0_1& s) : _set(s._set) {};

        disjoint_interval_set0_1(interval0_1<T> i)
        {
            if (i.min() < 0) i.min() = 0;
            if (i.max() < 0) i.max() = 0;
            if (i.min() > 1) i.min() = 1;
            if (i.max() > 1) i.max() = 1;
            _set.push_back(i);
        };

		value_type lower_limit() const { return empty() ? numeric_limits<T>::quiet_NaN() : _set.back().max(); };
		value_type upper_imit() const { return empty() ? numeric_limits<T>::quiet_NaN() : _set.front().min(); };
		bool contains(value_type v) const { return any_of(begin(), end(), [v](const interval0_1<T>& i) { return i.contains(v); }); };
		bool contains(const interval0_1<T>& v) const { return any_of(begin(), end(), [v](const interval0_1<T>& i) { return i.contains(v); }); };
		bool contains(const disjoint_interval_set0_1& v) const;
		bool empty() const { return _set.empty(); };
		const_iterator begin() const { return _set.begin(); };
		const_iterator end() const { return _set.end(); };

		// operators
		//disjoint_interval_set0_1 intersect(const disjoint_interval_set0_1& v) const { return (complement().unite(v.complement()).complement()); };
		//disjoint_interval_set0_1 symmetric_difference(const disjoint_interval_set0_1& v) const { return intersect(v.complement()).unite((v.intersect(complement()))); };
		//disjoint_interval_set0_1 complement() const { disjoint_interval_set0_1 comp; comp._set = complement_disjoint_interval_set(_set, T(0), T(1)); return comp; };
		//disjoint_interval_set0_1 difference(const disjoint_interval_set0_1& v) const { return intersect(v.complement()); };

		disjoint_interval_set0_1<T> unite(disjoint_interval_set0_1<T> v) const
		{
			disjoint_interval_set0_1<T> result(*this);
			result._set.insert(result._set.end(), v._set.begin(), v._set.end());
			result._set = make_disjoint_interval_set(result._set);
			return result;
		};

		disjoint_interval_set0_1<T> operator+(const disjoint_interval_set0_1& rhs)
		{
			disjoint_interval_set0_1<T> result;	
			for (auto x : _set)
				for (auto y : rhs._set)
					result._set.push_back(x + y);
			result._set = make_disjoint_interval_set(result._set);
			return result;
		};

    private:
        std::vector<interval0_1<T>> _set;
    };
}

#endif