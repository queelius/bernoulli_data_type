#pragma once

#include <memory>
#include <type_traits>

namespace alex::iterators
{
  /**
   * A forward_const_iterator<T> is a forward constant iterator type with
   * values of type T. Its constructor accepts any iterator that models (at
   * least) a forward iterator type and subsequently type-erases the iterator.
   */
  template <typename T>
  class forward_const_iterator
  {
  public:
    using value_type = T;
    using difference_type = long;
    using pointer = T const *;
    using reference = T const &;
    using iterator_category = std::forward_iterator_tag;    

    template <typename Iterator>
    forward_const_iterator(Iterator it) :
      _it(std::make_unique<concept_model<Iterator>>(std::move(it))) {};

    forward_const_iterator(forward_const_iterator const & copy)
    {
      _it = copy._it->copy();      
    };

    forward_const_iterator(forward_const_iterator && src) :
      _it(std::move(src._it)) {};

    forward_const_iterator & operator++()
    {
      _it->next();
      return *this;
    };

    forward_const_iterator operator++(int)
    {
      auto copy = *this;
      _it->next();
      return copy;
    };

    bool operator!=(forward_const_iterator const & other) const
    {
      return !_it->equals(other._it.get());
    };

    bool operator==(forward_const_iterator const & other) const
    {
      return _it->equals(other._it.get());
    };

    T operator*() const
    {
      return _it->value();
    };

    forward_const_iterator operator=(forward_const_iterator const & rhs)
    {
      _it = rhs._it->copy();      
      return *this;
    };

  private:
    struct concept
    {
      virtual void next() = 0;
      virtual bool equals(concept * const) const = 0;
      virtual T value() const = 0;
      virtual std::unique_ptr<concept> copy() const = 0;
    };

    template <typename Iterator>
    struct concept_model final : concept
    {
      concept_model(Iterator it) : it(std::move(it)) {};

      bool equals(concept * const other) const
      {
        return it == dynamic_cast<concept_model<Iterator>*>(other)->it;
      };

      T value() const
      {
        return *it;
      };

      void next()
      {
        ++it;
      };

      std::unique_ptr<concept> copy() const
      {
        return std::make_unique<concept_model<Iterator>>(it);
      };

      Iterator it;
    };

    std::unique_ptr<concept> _it;
  };

  template <typename Iterator>
  auto make_forward_const_iterator(Iterator it)
  {
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    return forward_const_iterator<value_type>(std::move(it));
  };
}