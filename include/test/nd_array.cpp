//#pragma once

#include <iostream>
#include <array>
#include <tuple>

/**
 * Let [q] denote {0,1,...,q-1} where q a positive integer and suppose we are
 * interested in a subset of functions ([X],[Y],[Z]) -> [X*Y*Z] that may be
 * used to map (x,y,z) coordinates in a 3d-array to an index into a 1d-array.
 * 
 * Since we wish to be able to access any element in the 3d-array, they are
 * total and bijective, and thus there are theoretically only (XYZ)! possible
 * functions that obey these contraints.
 * 
 * We are typically only interested in a few different mapping strategies.
 * Suppose f is an ordering strategy and we wish to layout the elements in a
 * 3d-array first by z, then by y, then by x.
 * 
 * Then, for any i in [X], j in [Y], and k in [Z]:
 * 
 *     f(i,j,k) < f(a,b,k+1)  for any a in [X] and b in [Y],
 *     f(i,j,k) < f(a,j,k)    for any a in [X], and
 *     f(i,j,k) < f(i,b,k)    for any b in {r in [Y] : r > j}.
 * 
 * The only function that obeys these contraints is implemented by the
 * xyz_layout.
 * 
 * 
 * A layout concept should also provide a means for an *optimal* traversal
 * of the elements in the underlying 1d-array. By optimality, we mean that
 * it is generally faster than other traversal strategies, most likely by
 * optimizing locality of reference, i.e., if sorted by z, then x, then
 * y, then first iterate over y's for a fixed z,x, then iterate by x
 * for a fixed z, and then iterate by z.
 */
template <int X, int Y, int Z>
struct zyx_layout
{
    int operator()(int i, int j, int k) const { return k*X*Y + j*X + i; }

    static constexpr int order[3] = {1,0,2};
};

/**
 * z,x,y layout is implemented by zxy_layout.
 */
template <int X, int Y, int Z>
struct zxy_layout
{
    int operator()(int i, int j, int k) const { return k*X*Y + i*Y + j; }
};

/**
 * x,y,z layout is implemented by xyz_layout.
 */
template <int X, int Y, int Z>
struct xyz_layout
{
    int operator()(int i, int j, int k) const { return i*Y*Z + j*Z + k; }
};




/**
 * An interesting case is given by *randomly* selecting an order function
 * from the (XYZ)! possibilities.
 */




template <typename T, int X, int Y, int Z, class Layout = zxy_layout<X,Y,Z>>
struct array_3d
{
    constexpr auto size() const { return X*Y*Z; }
    constexpr auto x() const { return X; }
    constexpr auto y() const { return Y; }
    constexpr auto z() const { return Z; }

    array_3d()
    {
        std::cout << "Oh...\n";
    };

    array_3d(array_3d const & t) : vs(t.vs)
    {
        std::cout << "Ah...\n";
    };

    template <class L2>
    array_3d(array_3d<T,X,Y,Z,L2> const & t)
    {
        std::cout << "Erm...\n";
        for (int i = 0; i < t.x(); ++i)
            for (int j = 0; j < t.y(); ++j)
                for (int k = 0; k < t.z(); ++k)
                    operator()(i,j,k) = t(i,j,k);
    }

    T operator()(int i, int j, int k) const { return vs[index(i,j,k)]; }
    T & operator()(int i, int j, int k) { return vs[index(i,j,k)]; }

    std::array<T, X*Y*Z> vs;
    Layout index;
};


template <int X, int Y, int Z>
struct arbitrary_layout
{
    // 3d-array should have a unique int at each component.
    3d_array<bounded_nat<X*Y*Z-1>,X,Y,Z> f;

    int operator()(int i, int j, int k) const
    {
        return f(i,j,k);
    }
};


template <typename T, int X, int Y, int Z, class Layout>
std::ostream& operator<<(std::ostream& out, array_3d<T,X,Y,Z,Layout> const & t)
{
    for (int k = 0; k < t.z(); ++k)
    {
        for (int i = 0; i < t.x(); ++i)
        {
            for (int j = 0; j < t.y(); ++j)
            {
                out << t(i,j,k) << "\t";
            }
            out << "\n";
        }
        out << "\n";
    }
    return out;
}


int main()
{
    array_3d<double, 2, 3, 4> t;

    double w = 0;
    for (int i = 0; i < t.z(); ++i)
    {
        for (int j = 0; j < t.x(); ++j)
        {
            for (int k = 0; k < t.y(); ++k)
            {
                t(j,k,i) = w++;
            }
        }
    }

    std::cout << t << "\n";

    std::cout << "\n---\n";

    for (auto x : t.vs)
        std::cout << x << " ";
    std::cout << "\n---\n";

    auto t2(t);
    std::cout << t2 << "\n";
    std::cout << "\n---\n";

    array_3d<double, 2, 3, 4, zyx_layout<2,3,4>> t3 = t;
    std::cout << t3 << "\n";
    std::cout << "\n---\n";
    for (auto x : t3.vs)
        std::cout << x << " ";
    std::cout << "\n---\n";


    array_3d<double, 2, 3, 4, xyz_layout<2,3,4>> t4 = t;
    std::cout << t4 << "\n";
    std::cout << "\n---\n";
    for (auto x : t4.vs)
        std::cout << x << " ";
    std::cout << "\n---\n";


    array_3d<double, 2, 3, 4, zxy_layout<2,3,4>> t5 = t4;
    std::cout << t5 << "\n";
    std::cout << "\n---\n";
    for (auto x : t5.vs)
        std::cout << x << " ";
    std::cout << "\n---\n";

}