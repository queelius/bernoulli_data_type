
template <unsigned int K, unsigned int B = 2u>
struct power_probability
{
    // static, compile time singleton data type for probability B^(-K).
    operator long double() const { return std::pow(B,-static_cast<int>(K)); }
};

template <unsigned int K1, unsigned int K2, unsigned int B>
constexpr power_probability<K1+K2,B> operator*(
    power_probability<K1,B>,
    power_probability<K2,B>)
{
    return {};
}

template <unsigned int K1, unsigned int K2, unsigned int B>
constexpr power_probability<K1-K2,B> operator/(
    power_probability<K1,B>,
    power_probability<K2,B>)
{
    return {};
}

template <unsigned int K1, unsigned int K2, unsigned int B>
constexpr auto operator<(
    power_probability<K1,B>,
    power_probability<K2,B>)
{
    return K1 > K2;
}

template <unsigned int K1, unsigned int K2, unsigned int B>
constexpr auto operator==(
    power_probability<K1,B>,
    power_probability<K2,B>)
{
    return K1 == K2;
}

template <unsigned int K1, unsigned int K2, unsigned int B>
constexpr auto operator>(
    power_probability<K1,B>,
    power_probability<K2,B>)
{
    return K1 < K2;
}

template <unsigned int K1, unsigned int K2, unsigned int B1, unsigned B2>
constexpr auto add(
    power_probability<K1,B1> a,
    power_probability<K2,B2> b)
{
    return static_cast<long double>(a) + static_cast<long double>(b);
}

