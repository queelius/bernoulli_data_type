#include <random>
#include <iostream>
#include <set>
#include <vector>

// what is the expected fp rate of the union of s1 and s2 given
// the union has tp true positives?
void expected_union_fp_rate(double e1, double e2, int U, int s1, int s2, int tp)
{
	std::random_device r;
	std::default_random_engine e(r());
	std::uniform_real_distribution<double> u(0, 1);
	std::uniform_int_distribution<int> du(0, U - 1);

	double u_fprate = 0;

	unsigned long long trial = 0;

	struct JointDist
	{
		bool s1_tp = false;
		bool s1_fp = false;
		bool s2_tp = false;
		bool s2_fp = false;
	};

	while (true)
	{
		std::vector<JointDist> univ(U);
		std::set<int> ss1;
		std::set<int> ss2;
		while (ss1.size() != s1)
		{
			auto x = du(e);
			ss1.insert(x);
			if (ss2.size() != tp)
			{
				ss2.insert(x);
			}
		}
		
		while (ss2.size() != s2)
		{
			auto x = du(e);
			if (ss1.count(x) == 0)
				ss2.insert(x);
		}

		for (auto x : ss1)
		{
			univ[x].s1_tp = true;
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

		// fp rate of union = P[x in (Sp1 or Sp2) | x not in (St1 or St2)].
		//                  = P[x in (Sp1 or Sp2) - (St1 or St2)] / tpcan gol
		double union_count = 0;
		for (const auto& x : univ)
		{
			if (!x.s1_tp || !x.s2_tp)
			{
				if (x.s1_fp || x.s2_fp)
					++union_count;
			}
		}
		double fp_rate = union_count / (U - tp);

		if (++trial % 1000 == 0)
		{
			std::cout << "union: " << u_fprate / trial << std::endl;
			std::cout << "		 " << (1 - (1 - e1) * (1 - e2)) << std::endl;
			std::cout << "		 " << (1 - (1 - e1) * (1 - e2)) * (1 - s1 / U) * (1 - s2 / U) << std::endl;
		}
	}
}
