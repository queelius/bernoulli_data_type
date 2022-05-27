#include <random>
#include <iostream>
#include <set>
#include <vector>

// what is the expected fp rate of the composition f2(f1(x))
// f1 : [x] -> [y], fp rate e1
// f2 : [y] -> [z], fp rate e2
// f2(f1(x)) : [x] -> [z], fp rate?
//
// X is size of [x]
// Y is size of [y]
// x is number of ?
void comp_fp_rate(double e1, double e2, int X, int Y, int x, int y)
{
	std::random_device r;
	std::default_random_engine e(r());
	std::uniform_real_distribution<double> u(0, 1);
	std::uniform_int_distribution<int> DUX(0, X - 1);
	std::uniform_int_distribution<int> DUY(0, Y - 1);

	unsigned long long trial = 0;

	while (true)
	{
		std::set<int> xx;
		std::set<int> yy;
		while (xx.size() != x)
		{
			auto x = DUX(e);
			// try to make x a key in f1
			xx.insert(x);
		}

		while (xx.size() != x)
		{
			auto y = DUY(e);
			// try to make y a key in f2
			yy.insert(x);
		}

		for (auto x : xx)
		{
			univX[x].s1_tp = true;
		}
		for (auto x : ss2)
		{
			univ[x].s2_tp = true;
		}

		// determine false positives for approximate sets
		// s1 and s2 given fp rates e1 and e2.
		for (auto& x : univ)
		{
			if (!x.s1_tp)
			{
				auto p = u(e);
				if (p <= e1)
				{
					x.s1_fp = true;
				}
			}
			if (!x.s2_tp)
			{
				auto p = u(e);
				if (p <= e2)
				{
					x.s2_fp = true;
				}
			}
		}
	}
}
