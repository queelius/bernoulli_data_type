#include <vector>
#include "disjoint_interval_set.hpp"
#include "interval.hpp"

namespace alex::math
{
	template <typename Interval  = interval<double>,
              typename Container = std::vector<Interval>>
	class disjoint_interval_set
	{
	public:
		using interval_type = Interval;
        using value_type = typename Interval::value_type;
		using const_iterator = typename Container::const_iterator;

		// constuctors
		disjoint_interval_set() {};
		disjoint_interval_set(const disjoint_interval_set& copy) : _set(copy._set) {};

		// accessors
		value_type lower_limit() const { return empty() ? numeric_limits<T>::quiet_NaN() : _set.back().max(); };
		value_type upper_imit() const { return empty() ? numeric_limits<T>::quiet_NaN() : _set.front().min(); };
		bool contains(value_type v) const { return any_of(begin(), end(), [v](const Interval& i) { return i.contains(v); }); };
		bool contains(const Interval& v) const { return any_of(begin(), end(), [v](const Interval& i) { return i.contains(v); }); };
		bool contains(const disjoint_interval_set& v) const;
		bool empty() const { return _set.empty(); };
		const_iterator begin() const { return _set.begin(); };
		const_iterator end() const { return _set.end(); };

		// operators
		disjoint_interval_set intersect(const disjoint_interval_set& v) const { return (complement().unite(v.complement()).complement()); };
		disjoint_interval_set symmetric_difference(const disjoint_interval_set& v) const { return intersect(v.complement()).unite((v.intersect(complement()))); };
		disjoint_interval_set complement(const disjoint_interval_set& v) const { disjoint_interval_set comp; comp._set = complement_disjoint_interval_set(_set); return comp; };
		disjoint_interval_set unite(disjoint_interval_set v) const;
		disjoint_interval_set difference() const { return intersect(v.complement()); };

	private:
		Container _set;
	};

    template <typename Interval,typename Container>
	bool
    disjoint_interval_set<Interval,Container>::contains(
        const disjoint_interval_set<Interval,Container>& v) const
	{
		auto i = begin();
		auto j = v.begin();
		while (i != end() && j != v.end())
		{
			if (i->contains(*j))
				++j;
			else
				++i;
		}
		return j == v.end();
	}


	// set-theoretic operations
	template <typename Interval,typename Container>
	disjoint_interval_set<Interval,Container>
    disjoint_interval_set<Interval,Container>::unite(
        disjoint_interval_set<Interval,Container> v) const
	{
		if (empty())
			return v;
		if (v.empty())
			return *this;

		v._set.insert(v._set.end(), _set.begin(), _set.end());
		return disjoint_interval_set<T>(v._set.begin(),
			make_disjoint_interval_set<T>(v._set.begin(), v._set.end()));
	}
}