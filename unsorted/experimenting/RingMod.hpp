#pragma once

#include <array>
#include <string.h>
#include <limits>

/**
 * RingMod<N> is Z modulo 2^N.
 * 
 * Most of the properties of Z modulo 2^N
 * are not implemented, only enough to be useful
 * as a set of hash values of arbitrary length
 * with multiplication, addition, and xor.
 * 
 * String literals for constructing common modulo
 * equivalence classes are provided, e.g.,
 *     101_mod16 => RingMod<4>(5).
 * 
 * RingMod<N> is a RegularType that overloads the
 * operator set {+, *, ^, ==, <}.
 */

namespace alex::ringmod
{
    // RingMod<N> is integer modulo 2^N
    template <size_t N>
    struct RingMod
    {
        auto begin() { return std::begin(digits); };
        auto end() { return std::end(digits); };
        auto log_mod() { return N; };

        RingMod()
        {
            std::array<bool,2>()
        }

        std::array<bool,N> digits;
    };

    // preconditions: x is a binary sequence of 0's or 1's.
    template <size_t N>
    RingMod<N> ring_mod_from_binary(const char* x)
    {
        auto n = strlen(x);
        if (n >= N) n = N;

        RingMod<N> a;
        for (size_t i = 0; i < n; ++i)
        {
            if (x[i] == '\0')
                break;

            // assert(x[i] == '0' || x[i] == '1');
            a.digits[n-i-1] = (bool)(x[i] - '0');
        }
        for (size_t i = n; i < N; ++i)
            a.digits[i] = false;
        return a;
    };

    template <size_t N>
    bool operator==(RingMod<N> a, RingMod<N> b)
    {
        return a.digits == b.digits;
    };

    template <size_t N>
    bool operator<(RingMod<N> a, RingMod<N> b)
    {
        for (size_t i=N-1; i < N; --i)
        {
            if (a.digits[i] != b.digits[i])
                return b.digits[i];
        }
        return false;
    };

    template <size_t N>
    RingMod<N> operator+(RingMod<N> a, RingMod<N> b)
    {
        return RingMod<N>{ detail::add_mod_n_op(a.digits, b.digits) };
    }

    template <size_t N>
    RingMod<N> operator^(RingMod<N> a, RingMod<N> b)
    {
        return RingMod<N>{ detail::xor_op(a.digits, b.digits) };
    }

    template <size_t N>
    RingMod<N> operator*(RingMod<N> a, RingMod<N> b)
    {
        return RingMod<N>{ detail::multiply_mod_n_op(a.digits, b.digits) };
    }

    RingMod<2> operator "" _mod4(const char* x)
    {
        return ring_mod_from_binary<2>(x);
    };

    RingMod<4> operator "" _mod16(const char* x)
    {
        return ring_mod_from_binary<4>(x);
    };

    RingMod<10> operator "" _mod10b(const char* x)
    {
        return ring_mod_from_binary<10>(x);
    };

    RingMod<512> operator "" _mod512b(const char* x)
    {
        return ring_mod_from_binary<512>(x);
    };

    // internal implementations
    namespace detail
    {
        template <size_t N>
        constexpr std::array<bool,N> xor_op(std::array<bool,N> a, std::array<bool,N> b)
        {
            std::array<bool,N> c{};
            for(size_t i = 0; i < N; ++i)
                c[i] = a[i] ^ b[i];
            return c;
        };

        template <size_t N>
        constexpr std::array<bool,N+1> add_op(std::array<bool,N> a, std::array<bool,N> b)
        {
            std::array<bool,N+1> c{};
            bool carry = false;
            for(size_t i = 0; i < N; ++i)
            {
                if (a[i])
                {
                    if (b[i])
                    {
                        c[i] = carry;
                        carry = true;
                    }
                    else
                        c[i] = !carry;
                }
                else if (b[i])
                    c[i] = !carry;
                else
                {
                    c[i] = carry;
                    carry = false;
                }
            }
            c[N] = carry;
            return c;
        };

        template <size_t N>
        constexpr std::array<bool,N> add_mod_n_op(std::array<bool,N> a, std::array<bool,N> b)
        {
            auto c = add_op(a, b);
            std::array<bool,N> d{};
            for (size_t i = 0; i < N; ++i)
                d[i] = c[i];
            return d;
        };

        template <size_t N>
        constexpr std::array<bool,2*N> multiply_op(std::array<bool,N> a, std::array<bool,N> b)
        {
            std::array<bool,2*N> c{};
            for(size_t i = 0; i < N; ++i)
            {
                std::array<bool,2*N> k{};
                for(size_t j = 0; j < N; ++j)
                    k[j+i] = a[i] && b[j];
                c = add_mod_n_op(c, k);
            }
            return c;
        };

        template <size_t N>
        constexpr std::array<bool,N> multiply_mod_n_op(std::array<bool,N> a, std::array<bool,N> b)
        {
            auto c = multiply_op(a, b);
            std::array<bool,N> d{};
            for (size_t i = 0; i < N; ++i)
                d[i] = c[i];
            return d;
        };
    }
}

//template<size_t N> class std::numeric_limits<alex::ringmod::RingMod<N>>
//{
//public:
//    static RingMod<N> lowest() { return RingMod<N>{} };
//    static RingMod<N> max() { return RingMod<N>{1} };
//};