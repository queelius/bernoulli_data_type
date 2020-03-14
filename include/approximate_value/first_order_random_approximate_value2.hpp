#pragma once

/**
 * a first-order approximate value of type X partitions the domain
 * into two parts.
 * 
 * we denote approx<T>
 */ 


/**
 * let's say we have some constructor 
 * 
 *     A+B
 * 
 * which creates a disjoint set of the elements in A and B.
 * 
 * Suppose we have a function not : {true} + {false} -> {true} + {false}
 * defined as
 * 
 *     id 1 := 1
 *     id 0 := 0
 * 
 * 
 * suppose we have an prefix-free coder for {true,false}, e.g.,
 *     enc(1) := {11}
 *     enc(0) := {0,10}
 * 
 * these codes are optimal if probability masses are given by
 *     p(1) = 1/4 
 *     P(0) = 3/4
 * 
 * consider id_1 has the probabilities
 * 
 * P[id_1(1) == 1] = 1-r
 * P[id_1(1) == 0] = r
 * 
 * where r is some rate distortion, often r=0.
 * 
 * P[id_1(0) == 0] = p(0) = 3/4
 * P[id_1(0) == 1] = 1-p(0) = p(1) = 1/4
 * 
 * so in this case, 1/4 is prob of id_1 not matching id on 0.
 * 
 * we denote id_{0,1} by id*.
 * 
 * 
 *  
 * 
 * if not[{true}][p] is not rated distorted over {true}, then
 *     P[not[{true}][p] true == false] = 1.
 * 
 * if it is rate-distorted, then
 *     P[not' true = false] = r, but it does
 * its best to find this mapping and it doesn't do anything to find
 * the mapping for not' false. its randomly mapped to a bit string which
 * is decoded to some value {true,false}. 
 * 
 * 
 */



template <typename T>
struct first_order_random_approximate_value
{
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