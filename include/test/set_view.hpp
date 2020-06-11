
#pragma once

#include <memory>
#include <algorithm>

using std::distance;
using std::binary_search;
using std::shared_ptr;

template <typename T>
class set_view
{
public:
    // if the container that the iterators point to is mutated, then the
    // iterator range [first, last) may be invalidated. this results in
    // undefined behavior for set_view.
    template <typename I>
    set_view(I first, I last) : concept_(new sorted_iterator_model<I>(first, last))
    {}

    auto operator()(T const & x) const
    {
        return concept_->operator()(x);
    }

    auto cardinality() const
    {
        return concept_->cardinality();
    }

private:
    struct concept
    {
        virtual bool operator()(T const &) const = 0;
        virtual double cardinality() const = 0;
    };

    template <typename I>
    struct sorted_iterator_model: public concept
    {
        sorted_iterator_model(I first, I last) : first(first), last(last) {}

        bool operator()(T const & x) const override
        {
            return binary_search(first, last, x);
        }

        double cardinality() const override
        {
            return distance(first,last);
        }

        I first, last;
    };

    shared_ptr<concept> const concept_;    
};