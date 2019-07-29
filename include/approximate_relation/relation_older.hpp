#ifndef __RELATION_HPP__
#define __RELATION_HPP__

// relations and relational algebra

#include <vector>
#include <initializer_list>
#include <iostream>

using std::ostream;
using std::istream;
using std::vector;
using std::initializer_list;
using std::pair;

namespace alex::collections
{
    template <typename U, typename V>
    ostream& operator<<(std::ostream& os, const std::pair<U, V>& p);

    template <typename T>
    class Relation
    {
    public:
        typedef size_t size_type;
        typedef typename vector<T>::iterator iterator;
        typedef typename vector<T>::const_iterator const_iterator;

        iterator begin() { return _v.begin(); }
        iterator end() { return _v.end(); }
        const_iterator cbegin() const { return _v.cbegin(); }
        const_iterator cend() const { return _v.cend(); }

        Relation(const Relation<T>& m) :
            _v(m._v), _rows(m._rows), _cols(m._cols)
        {
        };

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

        Relation(size_type rows = 0, size_type columns = 0) :
            _v(rows * columns),
            _rows(rows),
            _cols(columns)
        {
        };

        Relation(T val, size_type rows, size_type columns) :
            _v(rows * columns, val),
            _rows(rows),
            _cols(columns)
        {
        };

        size_type rows() const { return _rows; };
        size_type columns() const { return _cols; };
        T& operator()(size_type row, size_type col) { return _v[row * _cols + col]; };
        T operator()(size_type row, size_type col) const { return _v.at(row * _cols + col); }
        size_type size() const { return static_cast<size_type>(_v.size()); };
        T& operator()(size_type index) { return _v[index]; };
        T operator()(size_type index) const { return _v.at(index); };

        operator vector<T>() const { return _v; }

    private:
        vector<T> _v;
        size_type _rows, _cols;
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
    template <typename T>
    Relation<T> row_append(const Relation<T>& u, const Relation<T>& v)
    {
        typedef typename Relation<T>::size_type size_type;
        Relation<T> w(
            u.rows() + v.rows(),
            u.columns() < v.columns() ? v.columns() : u.columns()
        );

        for (size_type i = 0; i < u.rows(); ++i)
        {
            for (size_type j = 0; j < u.columns(); ++j)
            {
                w(i, j) = u(i, j);
            }
        }
        for (size_type i = 0; i < v.rows(); ++i)
        {
            for (size_type j = 0; j < v.columns(); ++j)
            {
                w(i + u.rows(), j) = v(i, j);
            }
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
    template <typename T>
    Relation<T> column_append(const Relation<T>& u, const Relation<T>& v)
    {
        typedef typename Relation<T>::size_type size_type;
        Relation<T> w(
            u.rows() < v.rows() ? v.rows() : u.rows(),
            u.columns() + v.columns()
        );

        for (size_type i = 0; i < u.rows(); ++i)
        {
            for (size_type j = 0; j < u.columns(); ++j)
            {
                w(i, j) = u(i, j);
            }
        }
        for (size_type i = 0; i < v.rows(); ++i)
        {
            for (size_type j = 0; j < v.columns(); ++j)
            {
                w(i, j + u.columns()) = v(i, j);
            }
        }
        return w;
    }


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
    Relation<T> transpose(const Relation<T>& t)
    {
        using Relation<T>::size_type;
        Relation<T> tt(t.columns(), t.rows());

        for (size_type i = 0; i < t.rows(); ++i)
        {
            for (size_type j = 0; j < t.columns(); ++j)
            {
                tt(j, i) = t(i, j);
            }
        }
        return tt;
    }


    // Relation<T> selection()
    // Relation<T> rename()
    // Relation<T> selection()
}

#endif