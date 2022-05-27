#include <random>
#include <iostream>
#include <set>
#include <vector>

void expected_overlap_discrete(double e1, double e2, int U, int s1, int s2);

void main()
{
	expected_overlap_discrete(.25, .25, 10000, 1000, 1000);
	std::system("pause");

}

void expected_overlap_discrete(double e1, double e2, int U, int s1, int s2)
{
	std::random_device r;
	std::default_random_engine e(r());
	std::uniform_real_distribution<double> u(0, 1);
	std::uniform_int_distribution<int> du(0, U-1);

	double i_fprate = 0;
	double i_tprate = 0;
	double i_fnrate = 0;
	double i_tnrate = 0;

	double u_fprate = 0;
	double u_tprate = 0;
	double u_fnrate = 0;
	double u_tnrate = 0;

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
			ss1.insert(du(e));
		}
		while (ss2.size() != s2)
		{
			ss2.insert(du(e));
		}

		for (auto x : ss1)
		{
			univ[x].s1_tp = true;
		}
		for (auto x : ss2)
		{
			univ[x].s2_tp = true;
		}

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

		double intersect_area = 0;
		double union_area = 0;
		// e_or = P[x in (Sp_1 or Sp_2) | x not in (St_1 or St_2)].
		// e_and = P[x in (Sp_1 and Sp_2) | x not in (St_1 and St_2)].

		double tp = 0;
		for (const auto& x : univ)
		{
			//std::cout << x.s1_tp << ", " << x.s1_fp << " : " << x.s2_tp << ", " << x.s2_fp << "\n";
			if (!x.s1_tp || !x.s2_tp)
			{
				if (x.s1_fp || x.s2_fp)
					++union_area;
			}
			if (!x.s1_tp && !x.s2_tp)
			{
				if (x.s1_fp && x.s2_fp)
					++intersect_area;
			}
			if (x.s1_tp && x.s2_tp)
				++tp;
		}
		intersect_area /= (U - tp);
		union_area /= U;

		i_fprate += intersect_area;
		u_fprate += union_area;

		if (++trial % 1000 == 0)
		{
			std::cout << "intersect: " << i_fprate / trial << std::endl;
			std::cout << "		     " << e1 * e2 << std::endl;
			std::cout << "		     " << ((U - s1 - s2 + tp) * e1 * e2 + (s1 - tp) * e1 + (s2 - tp) * e2) / (U - tp) << std::endl;
			std::cout << "union: " << u_fprate / trial << std::endl;
			std::cout << "		 " << (1 - (1 - e1) * (1 - e2)) << std::endl;
			std::cout << "		 " << (1 - (1 - e1) * (1 - e2)) * (1 - s1 / U) * (1 - s2 / U) << std::endl;
			//*s1 * s2 / U / U << std::endl;
		}
	}
}


void test1()
{
	// Seed with a real random value, if available
	std::random_device r;

	std::default_random_engine e(r());
	std::uniform_real_distribution<double> u(0, 1);

	const int N = 10000;
	double x = 0;
	for (int i = 0; i < N; ++i)
	{
		x += std::abs(u(e) - u(e));
	}
	std::cout << x / N << std::endl;
	std::system("pause");
}