#pragma once

#include <memory>

namespace bernoulli::set_indicator
{
    /**
     * Type-erasure for bernoulli values over the set-indicator function, i.e.,
     * a bernoulli value over type (2^X, contains : (2^X,X) -> bool).
     * 
     * The paramter T is the specific type returned by contains, i.e.,
     *     contains : (2^X,X) -> bernoulli<bool,T>.
     * A typical type for T may indicate a second-order positive bool, such
     * that the false negative rate is zero and the false positive rate is
     * some value in (0,1).
     * 
     * Note that bernoulli_set_indicator<X,T> models
     *     bernoulli<(2^X,X)->bool,
     */


    
    template <typename X, typename T>
    class bernoulli_set_indicator
    {
    public:
        using element_type = X;

        bernoulli_set_indicator(bernoulli_set const & copy) {};

        template <typename B>
        bernoulli_set_indicator(B const & b) : self_(std::make_shared<bernoulli_set_model<B>>(b))
        {
            // do nothing else        
        }

        auto contains(X const & x) const
        {
            return self_->contains(x);
        }

        auto fpr() const
        {
            return self_->fpr();
        }

        auto fpr() const
        {
            return self_->fnr();
        }

    private:
        struct bernoulli_set_concept
        {
            virtual bernoulli<bool,T> contains(X const &) const = 0;
            virtual error_rate_type fpr() const = 0;
            virtual error_rate_type fnr() const = 0;
        };

        template <typename B>
        struct bernoulli_set_model final : bernoulli_set_concept
        {
            bernoulli_set_model(B const & b) : b(b) {}

            bernoulli<bool,T> contains(T const & x) const override { return b.contains(x); }
            rate_type fpr() const override { return b.fpr(); }
            rate_type fnr() const override { return b.fnr(); }
            B const & b;
        };

        std::shared_ptr<bernoulli_set_concept const> self_;
    };
}
