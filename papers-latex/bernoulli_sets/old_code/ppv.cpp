#include <iostream>
#include <fstream>
#include <vector>
#include <random>

const int U = 10000;
const int p = 1000;
const int n = U - p;
const double lambda = (double)p / U;

double mean(std::vector<double> xs);
double var(std::vector<double> xs);
void ppv_1();
void ppv_2();
void ppv_3();
void ppv_4();

int main()
{
    ppv_1();
    ppv_2();
    ppv_3();
    ppv_4();
}

void ppv_1()
{
    const double fpr = 0.001;
    const double tpr = 0.999;

    const double tp = lambda * U * tpr;
    const double fp = U * (1-lambda) * fpr;
    const double vartp = (1-tpr) * tp;
    const double varfp = (1-fpr) * fp;

    const double asym_ppv = lambda * tpr / (lambda * (tpr - fpr) + fpr);

    std::default_random_engine gen;
    std::random_device rd;
    gen.seed(rd());
    std::binomial_distribution<int> FPn(n,fpr);
    std::binomial_distribution<int> TPp(p,tpr);

    std::ofstream out("ppv_1.dat");

    for (int i = 0; i < 10000; ++i)
    {
        auto fp = FPn(gen);
        auto tp = TPp(gen);
        auto ppv = (double)tp / (tp + fp);

        out << ppv << std::endl;
    }
}

void ppv_2()
{
    const double fpr = 0.025;
    const double tpr = 0.9;

    const double tp = lambda * U * tpr;
    const double fp = U * (1-lambda) * fpr;
    const double vartp = (1-tpr) * tp;
    const double varfp = (1-fpr) * fp;

    const double asym_ppv = lambda * tpr / (lambda * (tpr - fpr) + fpr);

    std::default_random_engine gen;
    std::random_device rd;
    gen.seed(rd());
    std::binomial_distribution<int> FPn(n,fpr);
    std::binomial_distribution<int> TPp(p,tpr);

    std::ofstream out("ppv_2.dat");

    for (int i = 0; i < 10000; ++i)
    {
        auto fp = FPn(gen);
        auto tp = TPp(gen);
        auto ppv = (double)tp / (tp + fp);

        out << ppv << std::endl;
    }
}


void ppv_3()
{
    const double fpr = 0.025;
    const double tpr = 0.999;

    const double tp = lambda * U * tpr;
    const double fp = U * (1-lambda) * fpr;
    const double vartp = (1-tpr) * tp;
    const double varfp = (1-fpr) * fp;

    const double asym_ppv = lambda * tpr / (lambda * (tpr - fpr) + fpr);

    std::default_random_engine gen;
    std::random_device rd;
    gen.seed(rd());
    std::binomial_distribution<int> FPn(n,fpr);
    std::binomial_distribution<int> TPp(p,tpr);

    std::ofstream out("ppv_3.dat");

    for (int i = 0; i < 10000; ++i)
    {
        auto fp = FPn(gen);
        auto tp = TPp(gen);
        auto ppv = (double)tp / (tp + fp);

        out << ppv << std::endl;
    }
}


void ppv_4()
{
    const double fpr = 0.01;
    const double tpr = 0.95;

    const double tp = lambda * U * tpr;
    const double fp = U * (1-lambda) * fpr;
    const double vartp = (1-tpr) * tp;
    const double varfp = (1-fpr) * fp;

    const double asym_ppv = lambda * tpr / (lambda * (tpr - fpr) + fpr);

    std::default_random_engine gen;
    std::random_device rd;
    gen.seed(rd());
    std::binomial_distribution<int> FPn(n,fpr);
    std::binomial_distribution<int> TPp(p,tpr);

    std::ofstream out("ppv_4.dat");

    for (int i = 0; i < 10000; ++i)
    {
        auto fp = FPn(gen);
        auto tp = TPp(gen);
        auto ppv = (double)tp / (tp + fp);

        out << ppv << std::endl;
    }
}



void pdf_ratio_norm()
{
    const int U = 1000;
    const int p = 100;
    const int n = U - p;
    const double lambda = (double)p / U;

    const double fpr = 0.01;
    const double tpr = 0.99;
    const double fnr = 1-tpr;
    const double tnr = 1-fpr;

    const double tp = lambda * U * tpr;
    const double fp = U * (1-lambda) * fpr;
    const double vartp = (1-tpr) * tp;
    const double varfp = (1-fpr) * fp;


}


void repeated_ppv_exp()
{
    const int U = 20;
    const int p = 3;
    const int n = U - p;
    const double lambda = (double)p / U;

    const double fpr = 0.01;
    const double tpr = 0.99;


    const double tp = lambda * U * tpr;
    const double fp = U * (1-lambda) * fpr;
    const double vartp = (1-tpr) * tp;
    const double varfp = (1-fpr) * fp;

    const double asym_ppv = lambda * tpr / (lambda * (tpr - fpr) + fpr);

    std::default_random_engine gen;
    std::random_device rd;
    gen.seed(rd());
    std::binomial_distribution<int> FPn(n,fpr);
    std::binomial_distribution<int> TPp(p,tpr);

    double total_delta_asym = 0;
    double total_delta_approx = 0;

    for (int j = 0; j < 1000; ++j)
    {
        std::vector<double> ppvs;

        for (int i = 0; i < U; ++i)
        {
            auto fp = FPn(gen);
            auto tp = TPp(gen);
            auto ppv = (double)tp / (tp + fp);

            ppvs.push_back(ppv);
        }

        auto approx_ppv = (tp / (tp + fp)) + (tp * varfp - fp * vartp) / std::pow(tp + fp, 3.);
        auto observed_ppv = mean(ppvs);

        auto delta_asym = std::abs(observed_ppv - asym_ppv);
        auto delta_approx = std::abs(observed_ppv - approx_ppv);

        total_delta_asym += delta_asym;
        total_delta_approx += delta_approx;

        //std::cout << "observed mean: " << observed_ppv << std::endl;
        //std::cout << "asym mean: " << asym_ppv << std::endl;
        //std::cout << "approx mean: " << approx_ppv << std::endl;
    }

    std::cout << total_delta_asym << std::endl;
    std::cout << total_delta_approx << std::endl;
}

double mean(std::vector<double> xs)
{
    double total = 0;
    for (auto x : xs)
    {
        total += x;
    }
    return total / xs.size();
}

double var(std::vector<double> xs)
{
    double total = 0;
    auto mu = mean(xs);
    for (auto x : xs)
    {
        total += (x - mu) * (x - mu);
    }
    return total / (xs.size() - 1);
}