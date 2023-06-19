#pragma once

/**
 * A realization of a value sampled from a random Bernoulli Boolean
 */
template <>
struct bernoulli<bool>
{
    bool value;
    rate_span err;
    bernoulli(rate_span err = rate_span()) : err(err) {}
    operator bool() const { return value; }
};

auto operator~(bernoulli<bool> a)
{
    return bernoulli<bool>{!a.value, a.err};
}

auto operator&(bernoulli<bool> a, bernoulli<bool> b)
{
    if (a.value && b.value)
    	return bernoulli<bool>{true,  a.err+b.err-a.err*b.err}

    auto err1 = b.err - a.err * b.err
    auto err2 = a.err - a.err + b.err
    auto err3 = 1.0f - a.err - b.err + a.err * b.err
    auto err_low = min(err1, err2, err3)
    auto err_hi = max(err1, err2, err3
    return bernoulli<bool>{false, std::make_pair(err_low, err_hi)}
}

auto operator|(bernoulli<bool> a, bernoulli<bool> b)
{
    // a or b = ((a or b)')' = (a' and b')'
    return !(!a & !b)
}

auto nor(bernoulli<bool> a, bernoulli<bool> b)
{
    return ~(a | b);
}

auto nand(bernoulli<bool> a, bernoulli<bool> b)
{
    return ~(a & b);
}

auto operator^(bernoulli<bool> a, bernoulli<bool> b)
{
    return (~a & b) | (a & ~b);
}

