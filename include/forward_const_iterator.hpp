#pragma once

#include <type_traits>

namespace alex::iterators {
/**
 * A forward_const_iterator<T> is a forward constant iterator type with
 * values of type T. Its constructor accepts any iterator that models (at
 * least) a forward iterator type and subsequently type-erases the iterator.
 */
template<typename T>
class forward_const_iterator
{
  public:
    using value_type = T;
    using difference_type = long;
    using pointer = T const*;
    using reference = T const&;
    using iterator_category = std::forward_iterator_tag;

    template<typename I>
    forward_const_iterator(I i)
      : i_(std::make_unique<concept_model<I>>(std::move(i))){};

    forward_const_iterator(forward_const_iterator const& copy)
    {
        i_ = copy.i_->copy();
    };

    forward_const_iterator& operator++()
    {
        i_->next();
        return *this;
    };

    forward_const_iterator operator++(int)
    {
        auto copy = *this;
        i_->next();
        return copy;
    };

    bool operator!=(forward_const_iterator const& other) const
    {
        return !i_->equals(other.i_.get());
    };

    bool operator==(forward_const_iterator const& other) const
    {
        return i_->equals(other.i_.get());
    };

    T operator*() const { return i_->value(); };

    forward_const_iterator operator=(forward_const_iterator const& rhs)
    {
        i_ = rhs.i_->copy();
        return *this;
    };

  private:
    struct concept
    {
        virtual void next() = 0;
        virtual bool equals(concept* const) const = 0;
        virtual T value() const = 0;
        virtual std::unique_ptr<concept> copy() const = 0;
    };

    template<typename I>
    struct concept_model final : concept
    {
        concept_model(I i)
          : i_(std::move(i)){};

        bool equals(concept* const other) const
        {
            return i_ == dynamic_cast<concept_model<I>*>(other)->i_;
        };

        T value() const { return *i_; };

        void next() { ++i_; };

        std::unique_ptr<concept> copy() const
        {
            return std::make_unique<concept_model<I>>(i_);
        };

        I i_;
    };

    std::unique_ptr<concept> i_;
};

template<typename I>
auto make_forward_const_iterator(I i)
{
    using value_type = typename std::iterator_traits<I>::value_type;
    return forward_const_iterator<value_type>(i);
};
}