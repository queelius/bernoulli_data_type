#pragma once
#include <memory>

/**
 * @brief Type-erasure for observed (Bernoulli) maps.
 * 
 * observed_map<X,Y> models the concept of an observed map (partial function)
 * of type X \-> Y where the mapping may have errors.
 * 
 * This represents a latent function f: X -> Y through its observed/approximate
 * version ~f: X -> Y that may have errors in its outputs.
 * 
 * By wrapping some type that models the concept and then subsequently erasing 
 * the specific type, this allows storing observed maps that vary over types 
 * into a container of observed_map<X,Y> elements.
 * 
 * As a model of a map, it is a functor that provides the member function
 *     Y operator()(X const &) const
 * to perform the mapping from X to Y.
 */
template <typename X, typename Y>
class observed_map
{
public:    
    using domain_type = X;
    using codomain_type = Y;

    template <typename F>
    observed_map(F const & f)
        : f_(std::make_shared<model_<F> const>(f)) {};

    observed_map(observed_map const & f) : f_(f.f_) {};

    auto operator()(X const & x) const { return f_->apply(x); }

    /**
     * Average error rate over entire domain, assuming that
     * elements in X are randomly chosen for f to be applied to.
     */
    auto error_rate() const { return f_->error_rate(); }

    /**
     * Retrieve the expected or observed error rate on element x.
     * Sometimes, this may not be knowable or may only be an estimate,
     * like an expectation.
     */
    auto error_rate(X const & x) const { return f_->error_rate(x); }

private:
    struct concept_
    {
        virtual ~concept_() = default;
        virtual Y apply(X const &) const = 0;
        virtual double error_rate(X const & x) const = 0;
        virtual double error_rate() const = 0;
    };

    template <typename F>
    struct model_ final : concept_
    {
        model_(F f) : f_(f) {}

        Y apply(X const & x) const override { return f_(x); }
        double error_rate(X const & x) const override { return f_.error_rate(x); }
        double error_rate() const override { return f_.error_rate(); }

        F f_;
    };

    std::shared_ptr<concept_ const> f_;
};

// Type alias for backward compatibility
template <typename X, typename Y>
using bernoulli_map = observed_map<X, Y>;
