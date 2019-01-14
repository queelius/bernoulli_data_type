#ifndef __DISJOINT_INTERVAL_SET_HPP__
#define __DISJOINT_INTERVAL_SET_HPP__

#include <vector>
#include <algorithm>
using std::sort;
using std::numeric_limits;

namespace alex::math
{
	template <typename Set>
	Set make_disjoint_interval_set(Set s)
	{
		using interval_type = typename Set::value_type;
		sort(s.begin(), s.end(), [](const interval_type& v1, const interval_type& v2)
		{
			return v1.min() < v2.min();
		});

		auto left = s.begin()->min();
		auto right = s.begin()->max();
		auto j = s.begin();
		for (const auto& i : s)
		{
			if (i.min() > right)
			{
				*j++ = interval_type(left, right);
				left = i.min();
				right = i.max();
			}
			else if (i.max() > right)
			{
				right = i.max();
			}
		}
		*j++ = interval_type(left, right);
		s.erase(j, s.end());
		return s;
	}

	template <typename Set1, typename Set2>
	auto unite_disjoint_interval_sets(
		Set1 s1,
		const Set2& s2)
	{
		if (s1.empty())
			return Set1(s2.begin(), s2.end());
		if (s2.empty())
			return s1;
		
		s1.insert(s1.end(), s2.begin(), s2.end());
		return make_disjoint_interval_set(s1);
	};

	template <typename Collection>
	using interval_type = typename Collection::value_type;

	template <typename Collection>
	using interval_value_type = typename interval_type<Collection>::value_type;

	/**
	 * @brief Takes the complement of a disjoint set of interval types.
	 * 
	 * @param s A disjoint set of intervals.
	 * @param l The lower limit of the set.
	 * @param u The upper limit of the set.
	 * @return The complement of s, with a lower limit l and an upper limit u.
	 */
	template <typename DisjointIntervalSet>
	DisjointIntervalSet complement_disjoint_interval_set(
		DisjointIntervalSet s,
		interval_value_type<DisjointIntervalSet> l = -std::numeric_limits<interval_value_type<DisjointIntervalSet>>::infinity(),
		interval_value_type<DisjointIntervalSet> u = std::numeric_limits<interval_value_type<DisjointIntervalSet>>::infinity())
	{
		using interval = interval_type<DisjointIntervalSet>;

		sort(s.begin(), s.end(),
			[](const interval& v1, const interval& v2)
				{ return v1.min() < v2.min();});

		DisjointIntervalSet comp;
		auto lr = l;
		for (const auto& i : s)
		{
			if (i.min() != l)
				comp.push_back(interval(lr, i.min()));
			lr = i.max();
		}
		if (lr != u)
			comp.push_back(interval(lr, u));
		return comp;
	};
}

#endif
