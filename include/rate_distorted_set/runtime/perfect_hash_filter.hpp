#pragma once

#include <set>

namespace alex::runtime::rate_distorted_set
{
    // Perfect hash filter (PHF): an implementation of the positive approximate
    // set ADT.
    template <
        typename T,
        template <typename> typename PerfHashFn,
        typename EncoderFn,
        typename DecoderFn,
        typename HashFn,
        typename ByteMatrix>    
    class perf_hash_filter
    {
        public:
            using value_type = T;
            using perf_hash_fn = PerfHashFn;
            using hash_fn = HashFn;
            using bit_matrix = BitMatrix;
            using encoder_fn = EncoderFn;
            using decoder_fn = DecoderFn;

            perf_hash_filter() {};
            perf_hash_filter(const perf_hash_filter& copy)
            {
            };

            perf_hash_filter(perf_hash_filter&& rval)
            {
            };

            perf_hash_filter& operator=(perf_hash_filter&& rval)
            {
                return *this;
            };

            perf_hash_filter& operator=(const perf_hash_filter& rhs)
            {
                return *this;
            };

            virtual ~perf_hash_filter() { clear(); };

            void clear()
            {
                if (_h.empty())
                {
                    _hashes.clear();
                    _ph.clear();
                }
            };

            bool operator()(const T& x) const
            {
            };

            bool contains(const T& x) const
            {
                // generate a hash of x. if x is in objective set, this is
                // a perfect hash with respect to other elements that are
                // in the objective set.
                auto l1 = _ph(x);

                //  generate another hash of x 
                auto l2 = _h(x) % _hashes.ncols();
                return true;
            };



            // this is an unbiased point estimator if the universal set
            // for type T is finite. Maybe look into conf. intervals
            // and return an alpha interval instead?
            auto fpr() const { return std::pow(2., -hash_width()); };
            auto fnr() const { return 0.; };

        private:
            static const unsigned int MAX_BYTE = (1 << CHAR_BIT);
        
            PerfHashFn ph_;
            HashFn h_;
            ByteMatrix hashes_;            
    };

    template <
        typename T,
        template <typename> typename PerfHashFn
    >
    class PerfectHashFilterBuilder
    {
    public:
        template <typename InputIterator>
        PerfectHashFilterBuilder<T>& set_keys(
            InputIterator begin,
            InputIterator end)
        {
            keys = std::set<T>(begin, end)
        };

        PerfectHashFilterBuilder<T>& set_fpr(double x)
        {
            if (x < 0.5) x = 0.5;
            if (x > 1.0) x = 1.0;

            width = static_cast<int>(std::round(-std::log(x)));
        };

        // each negative membership test is a bernoulli trial with a probability
        // of success in the set {2^-1=.5,2^-2=.25,2^-3=.125, ...}
        PerfectHashFilterBuilder<T>& set_hash_width(unsigned int width)
        {
            this->width = width;
        };

    private:
        std::set<T> keys;
        int width;
    };


    template <typename T>
    class PerfectHashFunctionBuilder
    {
    public:
    };
}
