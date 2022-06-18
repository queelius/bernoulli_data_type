#pragma once
#include <memory>

/**
 * @brief Type-erasure for bernoulli maps.
 * 
 * bernoulli_map<X,Y> models the concept of a Bernoulli map (partial function)
 * of type
 *     X \-> Y
 * by wrapping some type that models the concept and then
 * subsequently erases the specific type. This is known as type-erasure, and
 * allows, for instance, one to store Bernoulli maps that vary over types into
 * a container of bernoulli_map<X,Y> elements.
 * 
 * As a model of a map, it is a functor that provides the member function
 *     Y operator()(X const &) const
 * to perform the mapping from X to Y.
 */
template <typename X, typename Y>
class bernoulli_map
{
public:    
    using domain_type = X;
    using codomain_type = Y;

    template <typename F>
    bernoulli_map(F const & f)
        : f(std::make_shared<model<F> const>(f)) {};

    bernoulli_map(bernoulli_map const & f) : f(f.f) {};

    auto operator()(X const & x) const { return f(x); }
    auto error() const { return f->error(); }

private:
    struct concept_
    {
        virtual Y apply(X const &) const = 0;
        virtual double error() const = 0;
    };

    template <typename F>
    struct model_ final : concept_
    {
        model(F f) : f(f) {}

        Y apply(T const & x) const { return f(x); }
        double error() const { return f->error(); }

        F f;
    };

    std::shared_ptr<concept_ const> f;
};


