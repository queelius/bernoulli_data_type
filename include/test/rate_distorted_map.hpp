

#pragma once

/**
 * 
 */

template <typename E, typename D>
class rate_distorted_map
{
    using X = E::value_type;
    using Y = D::value_type;

    auto operator()(X const & x, Y const & y) const
    {
        auto h1 = hash(x);
        auto h2 = hash(y);

        auto s = shm(h1 ^ h2);
        return d(s);
    }

    D d;
    E e;

    // shm : hash_type -> hash_type
    singular_hash_map shm;
}

class commutative_rate_distorted_map
{
    auto operator()(auto x, auto y) const
    {
        auto h = hash(x);
        auto hh = hash(y);

        if (h < hh)
            h ^= hh;
        else
            h = hh ^ h;

        shm(h)
        

    }

    rate_distorted_map rdm;
}

/**
 * 
 *
 *
 */
