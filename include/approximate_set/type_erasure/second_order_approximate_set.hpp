/**
 * @file set.hpp Type-erased set open over types and closed over
 * some specified set of operators.
 * 
 * set<T> models a set with elements of type T by wrapping some
 * type that is able to model the set and then subsequently erases the
 * specific type. This is known as type-erasure, and allows, for instance,
 * one to store sets that vary over types into a container of
 * set<T> elements.
 * 
 * As a model of a set type, set<T> provides a predicate
 *     contains : (set<T>, T) -> bool
 * to answer queries about set membership.
 * 
 * There are many ways to represent a set, both mathematically and
 * representationally in computer memory. Depending upon the particulars of the
 * implementation, there may be functions, like union, that have more efficient
 * algorithms than the general case.
 * 
 * So, addition, set<T> has a well-defined overload set specified by an
 * overload_set_concept.
 * 
 * To take advantage of any such particulars for some function f, set<T>
 * must expose f through its overload_set_concept. A concrete
 * overload_set_model<Set> implements the concept.
 *
 * If the sets are different, then binary operators like + are problematic,
 * i.e., + : (T1,T2) -> T3, where T1,T2,T3 may all be different.
 * 
 * If we want to deal with this in some way other than returning Nothing,
 * we delegate the operation to a function of the type
 *     + : (set<T>, set<T>) -> set<T>,
 * which may use a multiple dispatch method over a closed set of types.
 * 
 * One may ask, why not just use variant or something similar? Variant is
 * is closed over the types while the operations are an open set. set<T>
 * is open over the types with a closed set of operations. Indeed, this is
 * how subtyping in C++ and other languages in general work, but set<T>
 * provides value semantics and it does not require that all of the types
 * inherit from some base class (set_base). Rather, we require that the
 * type adheres to some set of *concepts*, namely, the overload set of
 * operators required by overload_set_concept.
 */

#pragma once

#include <memory>

using std::shared_ptr;
using std::make_shared;

template <typename X>
class second_order_approximate_set
{
public:
    // a first-order random approximate set may be generated, for instance,
    // from a binary symmetric channel.
    using value_type = X;

    template <typename ASet>
    second_order_approximate_set(ASet s)
        : s_(make_shared<model<ASet> const>(s)) {};

    second_order_approximate_set(second_order_approximate_set const & s) : s_(s._s) {};

    auto contains(T const & x) const { return s_->contains(x); }
    auto operator()(T const & x) const { return s_->contains(x); }
    auto operator==(second_order_approximate_set const & a) const { return s_->eq(a); }
    auto operator!=(second_order_approximate_set const & a) const { return !s_->eq(a); }
    auto size() const { return s_->size(); }
    auto fpr() const { return s_->fpr(); }
    auto fnr() const { return s_->fnr(); }

private:
    struct concept
    {
        virtual bool operator()(T const &) const = 0;

        // Given two second-order random approximate sets over X,
        // consider the following two facts:
        //
        // (1) The probability that they are equal goes to zero as
        // |X| goes to infinity.
        // 
        // (2) Representational equality => equality.
        // 
        // For a finite univeral set, equality is possible but
        // as an approximation, we say two approximations are
        // equal iff they have the same representation.        
        virtual bool eq(second_order_approximate_set<T> const &) const = 0;

        // A second-order random approximate set, denoted by s, has false positive rates and false negative
        // rates are a priori asymptotically normally distributed with means given respectively by
        // fpr(s) and fnr(s).
        virtual double fpr() const = 0;
        virtual double fnr() const = 0;
    };

    template <typename ASet>
    struct model final : concept
    {
        model(ASet s) : s_(s) {}

        approximate<2,bool> contains(T const & x) const { return s_->contains(x); }

        double fpr() const { return s_->fpr(); }

        double fnr() const { return s_->fnr(); }

        approximate<-1,bool> eq(second_order_approximate_set const & t) const
        {
            auto p = t.s_.get();
            if (auto k = dynamic_cast<model<ASet> const *>(p); k)
                return s_ == k->s_;

            return false; // overload_set::eq(matrix<T>, matrix<T>);
        }

        Set s_;
    };

    shared_ptr<concept const> s_;
};
