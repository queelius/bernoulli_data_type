#include <boost/numeric/interval.hpp>
#include <iostream>
#include <fstream>
#include <variant>
using interval = boost::numeric::interval<double>;

void test_approx_ppv_vs_u(double lambda = .2, std::ostream& o = std::cout)
{
	interval fpr(.1, .1);
	interval fnr(0, 0);
	const interval one(1, 1);

	for (double u = 10; u <= 1000; u += 10)
	{
		auto p = lambda * u;
		auto n = (1. - lambda) * u;

		auto tp = p * (one - fnr);
		auto fp = n * fpr;
		auto varfp = p * (one - fnr) * fnr;
		auto vartp = n * fpr * (one - fpr);
		auto ppv = tp / (tp + fp) + (tp * varfp - fp * vartp) / ((tp + fp)*(tp + fp)*(tp + fp));

		auto ppv_asymptotic = tp / (tp + fp);

		o << ppv.lower() << ',' << ppv.upper()
		  << " (" << ppv_asymptotic.lower() << ',' << ppv_asymptotic.upper() << ")\n";
	}
}

void test_ppv_vs_u(double lambda = .2, std::ostream& o = std::cout)
{
	interval v(.1, .1);
	interval w(0, 0);
	const interval one(1, 1);

	for (double u = 10; u <= 1000; u += 10)
	{
		auto p = lambda * u;
		auto n = (1. - lambda) * u;

		auto tp = p * (one - w);
		auto fp = n * v;
		auto varfp = p * (one - w) * w;
		auto vartp = n * v * (one - v);
		auto ppv = tp / (tp + fp) + (tp * varfp - fp * vartp) / ((tp + fp)*(tp + fp)*(tp + fp));

		auto ppv_asymptotic = tp / (tp + fp);

		o << ppv.lower() << ',' << ppv.upper()
			<< " (" << ppv_asymptotic.lower() << ',' << ppv_asymptotic.upper() << ")\n";
	}
}

interval accuracy(interval lambda, interval fpr, interval fnr)
{
	const interval one(1, 1);
	return lambda * (one - fnr) + (one - lambda) * (one - fpr);
};

double accuracy_exact(double lambda, double fpr, double fnr)
{
	const double one(1);
	return lambda * (one - fnr) + (one - lambda) * (one - fpr);
};

interval accuracy_exact2(interval lambda, double fpr, double fnr)
{
	const double one(1);
	return one + lambda * (fpr - fnr) - fpr;
};


int main(int argc, char** argv)
{


	auto acc = accuracy(interval(0, 1), interval(.1,.1), interval(0,0));	
	std::cout << "accuracy: " << acc.lower() << ", " << acc.upper() << std::endl;
	auto acc2 = accuracy_exact2(interval(0, 1), .1, 0);
	std::cout << "accuracy: " << acc2.lower() << ", " << acc2.upper() << std::endl;


	for (double lam = 0; lam <= 1; lam += 0.05)
	{
		auto acc_ex = accuracy_exact(lam, .1, 0);
		std::cout << "\taccuracy exact: " << acc_ex << std::endl;
	}
}
