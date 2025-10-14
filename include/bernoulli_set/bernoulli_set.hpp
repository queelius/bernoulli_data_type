#pragma once

/**
 * @file observed_set.hpp Type-erased observed set implementation.
 * 
 * observed_set<X> models the concept of an observed set ~S where S is the
 * latent/true set. The observed set may have false positives and false
 * negatives when testing membership.
 * 
 * This implementation uses type-erasure to allow storing different concrete
 * observed set implementations in the same container.
 * 
 * As a model of a set type, observed_set<T> provides a predicate
 *     contains : (observed_set<T>, T) -> bool
 * to answer queries about set membership (with possible errors).
 * 
 * The latent/observed framework:
 * - S represents the true/latent set
 * - ~S represents the observed/approximate set
 * - Membership queries to ~S may return incorrect results with rates α (FPR) and β (FNR)
 */

#include <memory>
#include "bernoulli_bool/bernoulli_boolean.hpp"
#include "rate_span.hpp"

// Forward declaration for expression templates
namespace algebra {
    template <typename Derived>
    class bernoulli_set_expr;
}

/**
 * Models an observed set ~S with elements of type X.
 *
 * The observed set represents a latent set S through noisy observations.
 * Membership queries have:
 * - False positive rate α: P(x ∈ ~S | x ∉ S)
 * - False negative rate β: P(x ∉ ~S | x ∈ S)
 *
 * Note that observed_set<X> is *non-iterable*, even though concrete 
 * implementations may support iteration.
 *
 * observed_set<X> is not a regular type since equality would itself
 * return an uncertain result (observed bool).
 */
template <typename X>
class observed_set : public algebra::bernoulli_set_expr<observed_set<X>>
{
public:
    using value_type = X;

    template <typename B>
    observed_set(B const & s)
        : s_(std::make_shared<model_<B> const>(s)) {}

    observed_set(observed_set const & s) : s_(s.s_) {}

    /**
     * Test if x is in the observed set ~S.
     * May return incorrect results with rates α and β.
     */
    auto contains(X const & x) const { return s_->contains(x); }
    auto operator()(X const & x) const { return contains(x); }
    
    /**
     * Get the false positive rate α = P(x ∈ ~S | x ∉ S)
     */
    auto false_positive_rate() const { return s_->fpr(); }
    
    /**
     * Get the false negative rate β = P(x ∉ ~S | x ∈ S)
     */
    auto false_negative_rate() const { return s_->fnr(); }

private:
    struct concept_
    {
        virtual ~concept_() = default;
        virtual bool contains(X const &) const = 0;
        virtual rate_span fpr() const = 0;
        virtual rate_span fnr() const = 0;
    };

    template <typename B>
    struct model_ final : concept_
    {
        model_(B s) : s_(s) {}

        bool contains(X const & x) const override { return s_.contains(x); }
        rate_span fpr() const override { return s_.false_positive_rate(); }
        rate_span fnr() const override { return s_.false_negative_rate(); }

        B s_;
    };

    std::shared_ptr<concept_ const> s_;
};

// Type alias for backward compatibility
template <typename X>
using bernoulli_set = observed_set<X>;