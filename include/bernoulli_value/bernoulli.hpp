#pragma once

namespace bernoulli
{
    template <typename P, typename T, typename F>
    struct bernoulli {};

    // partitions. these are reasonable for booleans or sets where positives
    // are true or map to true on the set-indicator function.
    struct pos_neg {};
    struct pos {};
    struct neg {};
    struct universal {};

    // order
    struct first_order
    {
        using partition_type = universal;
        constexpr auto order() const { return 1; }
    };

    template <typename P>
    struct second_order
    {
        using partition_type = P;
        constexpr auto order() const { return 2; }
    };

    template <typename P>
    struct higher_order
    {
        using partition_type = P;
        auto order() const { return k; }

        int k;
    };

    // operations / functions that the bernoulli error is naturally defined
    // with respect to. note that other operations or functions may also have
    // a type of error induced by the bernoulli error on the natural operation.
    struct eq {};

    // std::equal_to<T>
}