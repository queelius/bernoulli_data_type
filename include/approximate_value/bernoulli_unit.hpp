#pragma once

/**
 * The unit type is any type with only a single value in its set of possible values.
 * 
 * By an accident of history, the unit type is denoted by the keyword 'void' in
 * C++, which is the absurd type (empty set). Additionally, in C++, we cannot
 * use values of the unit type in an expression. So, for the sake of
 * completeness, we specify a unit type
 *     struct unit{}.
 * 
 * If a Bernoulli of the unit is to realize values of the unit type, it is not
 * possible to have errors -- it must realize the value unit(). Thus, the *only*
 * Bernoulli model for the unit type is the zero-th order model.
 */
struct unit {};

struct random_bernoulli<0,unit>
{
    using value_type = unit;
    static auto order() { return 0; }

    static auto fpr() const { return 0.; }
    static auto fnr() const { return 0.; }
    static auto error(unit const &) const { return 0.; }

    auto operator()() const { return unit{}; }

    template <typename R>
    auto sample(R &) { return unit{}; }
}

/**
 * If we take the sum of unit type, we get a type that is isomorphic
 * to the Boolean type. This is a foundational type, as Bernoullis of this
 * type may be used to generate every other type, e.g., the Bernoulli set
 * may be modeled as an array of Bernoulli Booleans over a particular Boolean
 * algebra.
 */

template <typename L, typename R>
struct sum
{
    using type = std::variant<L,R>;
};

template <>
struct sum<random_bernoulli<0,unit>,random_bernoulli<0,unit>>
{
    using type = random_bernoulli<2,bool>;
};