
/**
 * Suppose R is a real number. Then,
 *     auto floor(power_of_2_probability<R>) { return power_of_2_probability<floor(R)>{}; }
 *     auto power_of_2_probability<R> ceil(power_of_2_probability<R>) { return power_of_2_probability<floor(R)>{}; } 
 *     auto round(power_of_2_probability<R> r) { return R < 1 - log2(3) ? floor(r) : ceil(r); }
 * 
 * template <typename T, unsigned int K>
 * convert(T x) { return power_of_2_probability}
 */




template <unsigned int K>
struct power_of_2_probability
{
    // static, compile time singleton data type for probability 2^(-K).
};


template <unsigned int K1, unsigned int K2>
power_of_2_probability<K1+K2> operator*(
    power_of_2_probability<K1>,
    power_of_2_probability<K2>)
{
    return {};
}

template <unsigned int K1, unsigned int K2>
power_of_2_probability<K1-K2> operator/(
    power_of_2_probability<K1>,
    power_of_2_probability<K2>)
{
    return {};
}


template <unsigned int K1, unsigned int K2>
auto operator<(
    power_of_2_probability<K1>,
    power_of_2_probability<K2>)
{
    return K1 > K2;
}

