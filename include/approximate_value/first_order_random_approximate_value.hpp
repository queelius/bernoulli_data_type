#pragma once

/**
 * Make this a Random Variable type.
 */ 

template <typename T>
struct first_order_random_approximate_value
{
    double fnr() const { return fnr_; };
    double fpr() const { return fpr_; };
    T value() const { return value_; };

    double fpr_;
    double fnr_;
    T value_;

    // negative set should be identically distributed
};

template <typename T, typename FNR, typename FPR>
struct second_order_random_approximate_value
{
    FNR fnr() const { return fnr_; };
    FNR fpr() const { return fpr_; };

    T value;
};

template <typename T, typename FNR, typename FPR>
struct higher_order_random_approximate_value
{
    FNR fnr() const { return fnr_; };
    FNR fpr() const { return fpr_; };

    T value;
};


template <typename T, typename FNR, typename FPR>
auto false_positive_rate(first_order_random_approximate_value<T,FNR,FPR> x)
{
    // k is |X| - |A| 
    return 0; // binomial<FPR>(k, x.fpr())
}




/**
 * If we have a function f : 
 * 
 */


/**
 * For higher-order, maybe interval stuff? does normal approx still work?
 */