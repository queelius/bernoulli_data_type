// relations and relational algebra

#pragma once

#include <vector>
#include <tuple>
#include <iostream>

namespace alex::relation
{
    template <typename ... Ts>
    class relation;

    template <typename T>
    using unary_relation = relation<std::tuple<T>>;

    template <typename T, typename U>
    using binary_relation = relation<std::tuple<T,U>>;

    template <typename T, typename U, typename V>
    using ternary_relation = relation<std::tuple<T,U,V>>;

    template <typename...Ts>
    class relation
    {
    public:
        using size_type = size_t;
        using tuple_type = std::tuple<Ts...>;
        using iterator = typename vector<tuple_type>::iterator;
        using const_iterator = typename vector<tuple_type>::const_iterator;

        iterator begin() { return _v.begin(); }
        iterator end() { return _v.end(); }
        const_iterator cbegin() const { return _v.cbegin(); }
        const_iterator cend() const { return _v.cend(); }

        relation(relation const & copy) : _v(copy._v) {};
        relation(size_type rows = 0) : _v(rows) {};

        size_type nr() const { return _v.size(); };
        size_type nc() const { return std::tuple_size<tuple_type>::value; };

        template <size_t Row, size_t Column>
        auto operator()() const { return std::get<Column>(_v.at(Row)); };

        tuple_type& operator()(size_type index) { return _v[index]; };
        tuple_type operator()(size_type index) const { return _v.at(index); };

    private:
        std::vector<tuple_type> _v;
    };




    // if u =
    // [
    //     [1 2 3]
    //     [4 5 6]
    // ]
    // and v =
    // [
    //     [a b]
    //     [c d]
    //     [e f]
    // ],
    // then row_append(u, v) =
    // [
    //     [1 2 3]
    //     [4 5 6]
    //     [a b *]
    //     [c d *]
    //     [e f *]
    // ],
    // where * is a default initialized value.
    template <typename ... Ts>
    relation<Ts...> row_append(const relation<Ts...>& u, const relation<Ts...>& v)
    {
        typedef typename Relation<T>::size_type size_type;
        relation<Ts...> w(u.rows() + v.rows());

        for (size_type i = 0; i < u.rows(); ++i)
        {
          w(i, j) = u(i, j);
        }
        for (size_type i = 0; i < v.rows(); ++i)
        {
          w(i + u.rows(), j) = v(i, j);
        }

        return w;
    }

    // if u =
    // [
    //     [1 2 3]
    //     [4 5 6]
    // ]
    // and v =
    // [
    //     [a b]
    //     [c d]
    //     [e f]
    // ],
    // then column_append(u, v) =
    // [
    //     [1 2 3 a b]
    //     [4 5 6 c d]
    //     [* * * e f]
    // ],
    // where * is a default initialized value.
    template <typename...Us, typename ... Vs>
    auto column_append(const relation<Us...>& u, const relation<Vs...>& v)
    {
        relation<Us...,Vs...> w(
            u.nrows() < v.nrows() ? v.nrows() : u.nrows(),
        );

        for (size_t i = 0; i < u.nrows(); ++i)
        {
                w(i, j) = u(i, j);
        }
        for (size_type i = 0; i < v.rows(); ++i)
        {
                w(i, j + u.columns()) = v(i, j);
        }
        return w;
    };


    // if u = [1 2 3]
    // and v = [a b]
    // then cartesian_product(u, v) =
    // [
    //     [(1,a) (1,b)]
    //     [(2,a) (2,b)]
    //     [(3,a) (3,b)]
    // ].

    template <typename T>
    using iterator_value_type = typename std::iterator_traits<typename T::iterator>::value_type;

    template <typename X, typename Y>
    auto cartesian_product(const X& xs, const Y& ys) ->
        Relation<pair<iterator_value_type<X>, iterator_value_type<Y>>>
    {
        const auto n = std::distance(xs.begin(), xs.end());
        const auto m = std::distance(ys.begin(), ys.end());
        Relation<pair<iterator_value_type<X>, iterator_value_type<Y>>> prod(n, m);

        auto cur = prod.begin();
        for (auto x : xs)
        {
            for (auto y : ys)
            {
                *cur++ = std::make_pair(x, y);
            }
        }
        return prod;
    }

    template <typename U, typename V>
    ostream& operator<<(ostream& os, const pair<U, V>& p)
    {
        os << "(" << p.first << ", " << p.second << ")";
        return os;
    }

    template <typename T>
    ostream& operator<<(ostream& out, const Relation<T>& t)
    {
        typedef typename Relation<T>::size_type size_type;

        out << "{\n";
        for (size_type i = 0; i < t.rows(); ++i)
        {
            if (i != 0)
                out << ",\n";
            out << "\t{";
            for (size_type j = 0; j < t.columns(); ++j)
            {
                if (j != 0)
                    out << ",\t";
                out << t(i, j);
            }
            out << '}';
        }
        out << "\n}";
        return out;
    }

    template <typename T>
    istream& operator>>(istream& out, Relation<T>& t)
    {
        auto cur = t.begin();
        while (out >> *cur++ && cur != t.end());
        return out;
    }

    template <typename T>
    relation<T> transpose(relation<T> const & r)
    {
        using relation<T>::size_type;
        relation<T> rt(r.ncols(), r.nrows());

        for (size_type i = 0; i < t.nrows(); ++i)
        {
            for (size_type j = 0; j < t.ncols(); ++j)
            {
                rt(j, i) = r(i, j);
            }
        }
        return rt;
    };


    template <typename Relation>
    class relation_tranpose
    {
        
    };

    template <typename Relation>
    class relation_selection
    {
        
    };


    template <typename Relation>
    relation_selection<Relation> select(std::function<bool(typename Relation::tuple_type))
    {

    };
    // Relation<T> rename()
    // Relation<T> selection()






















}














/*
        template <typename Iter>
        Relation(Iter begin, Iter end, size_type rows)
        {
            _rows = rows;
            for (auto x = begin; x != end; ++x)
            {
                _v.push_back(*x);
                ++_cols;
            }
            _cols = static_cast<size_type>(ceil(static_cast<float>(_cols) / rows));
            _v.resize(_rows * _cols);
        };

        Relation(initializer_list<initializer_list<T>> m) : _rows(0), _cols(0)
        {
            auto first_row = true;
            for (auto row : m)
            {
                ++_rows;
                size_type this_cols = 0;
                for (auto x : row)
                {
                    ++this_cols;
                    if (first_row)
                        ++_cols;
                    _v.push_back(x);
                }
                first_row = false;
                if (this_cols != _cols)
                    throw std::invalid_argument("[Relation<T>::ctor] Invalid Initializer List");
            }
        }

        Relation(initializer_list<T> t, size_type rows = 1) :
            _v(t.begin(), t.end()),
            _rows(rows),
            _cols(static_cast<size_type>(ceil(static_cast<float>(t.size()) / rows)))
        {
            _v.resize(_rows * _cols);
        };
*/