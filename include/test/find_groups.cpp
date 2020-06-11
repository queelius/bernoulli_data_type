#include <iostream>
#include <vector>
#include "bounded_nat.hpp"
#include "table.hpp"
#include "set_view.hpp"
#include "finite_partial_binary_fn.hpp"

std::ostream& operator<<(std::ostream& out, bounded_nat const & x)
{
    out << x.get() << "\n";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> const & xs)
{
    for (auto x : xs)
        out << x << "\n";
    return out;
}

template <typename X,typename Y,typename Z>
std::ostream& operator<<(std::ostream& out, std::tuple<X,Y,Z> const & t)
{
    out << "<" << std::get<0>(t) << "," << std::get<1>(t) << "," << std::get<2>(t) << ">";
    return out;
}

template <typename X,typename Y>
std::ostream& operator<<(std::ostream& out, std::pair<X,Y> const & t)
{
    out << "<" << t.first << "," << t.second << ">";
    return out;
}



template <typename T>
std::ostream& operator<<(std::ostream& out, table<T> const & t)
{
    for (int i = 0; i < t.nr(); ++i)
    {
        for (int j = 0; j < t.nc(); ++j)
        {
            out << t(i,j) << "\t";
        }
        out << "\n";
    }
    return out;
}

std::vector<table<int>> find_groups_helper(table<int> t, int ii)
{
    int order = t.nc();
    std::vector<table<int>> gs;
    if (ii == order*order)
    {
        gs.push_back(t);
        return gs;
    }    

    for (int i = ii; i < (int)t.size(); ++i)    
    {
        if (t(i) == 0)
        {
            for (int k = 1; k <= order; ++k)
            {
                table<int> tt(t);
                tt(i) = k;
                auto ggs = find_groups_helper(tt, ii+1);
                gs.insert(gs.end(),ggs.begin(),ggs.end());
            }
        }
    }
    return gs;
}

std::vector<table<int>> find_groups(int order)
{
    return find_groups_helper(table<int>(order,order), 0);
}


template <typename T>
bool is_group_isomorphic(table<T> g1, table<T> g2)
{
    return true;
}

int main()
{
    std::vector<std::tuple<int,int,int>> fs{
        {1,0,1},{1,1,1},{1,2,0},
        {0,0,0},{0,1,1},{0,2,2},
        {2,0,2},{2,1,0},{2,2,1}
    };
    finite_partial_binary_fn<int,int,int> f(fs.begin(),fs.end());

    auto t = get<3>(to_table(f));
    std::cout << t << "\n\n";

    std::cout << "---\ncommutative? " << is_commutative(f) << "\n";
    std::cout << "---\nassociative? " << is_associative(f) << "\n";


    exit(0);


    auto gs = find_groups(2);

    int i = 0;
    for (auto g : gs)
    {
        std::cout << "group " << ++i << "\n";
        std::cout << g << "\n\n";
    }
}





/*

std::vector<Group> find_subgroups(Group g)
{
    // lagranges thereom states that if h <= g => |h| divides |g|.
    // so any h such that |h| does not divide |g| is not a subgroup. only need
    // to look at the divisors of |g| for possible subgroups.
}

bool is_subgroup(Group h, Group g)
{
    // quick check: if |h| does not divide |g|, then false. 
}


bool is_group(finite_binary_function f)
{
    return true;
}

bool is_semi_group(finite_binary_function f)
{
    return true;
}


bool is_monoid(finite_binary_function f)
{
    return true;
}


bool is_magma(finite_binary_function f)
{
    return true;
}

bool is_ring(finite_binary_function f)
{
    return true;
}

bool is_semiring(finite_binary_function f)
{
    return true;
}
*/

