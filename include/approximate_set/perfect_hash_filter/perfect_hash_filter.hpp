#ifndef __PERFECT_HASH_FILTER_HPP__
#define __PERFECT_HASH_FILTER_HPP__

#include <set>

namespace alex::approximate_sets
{
    // Perfect hash filter (PHF): an implementation of the positive approximate
    // set ADT.
    template <
        typename T,
        template <class> class PerfectHashFunc,
        class Coder,
        class HashFunc,
        class BitMatrix>    
    class PerfectHashFilter
    {
        public:
            using value_type = T;
            using perfect_hash_func = PerfectHashFunc;
            using hash_func = HashFunc;
            using bit_matrix = BitMatrix;

            PerfectHashFilter() {};
            PerfectHashFilter(const PerfectHashFilter& copy)
            {
            };

            PerfectHashFilter(PerfectHashFilter&& rval)
            {
            };

            PerfectHashFilter& operator=(PerfectHashFilter&& rval)
            {
                if (this != &other)
                {
                }
                return *this;
            };

            PerfectHashFilter& operator=(const PerfectHashFilter& rhs)
            {
                if (this != &other)
                {
                }
                return *this;
            };

            virtual ~PerfectHashFilter() { clear(); };

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
                auto idx = _ph(x);

                //  generate another hash of x 
                auto hash = _h(x) % _hashes.ncols();
                return true;
            };



            // this is an unbiased point estimator if the universal set
            // for type T is finite. Maybe look into conf. intervals
            // and return an alpha interval instead?
            auto fpr() const { return std::pow(2., -hash_width()); };

        private:
            static const unsigned int MAX_BYTE = (1 << CHAR_BIT);
        
            PerfectHashFunc _ph;
            BitMatrix _hashes;
            HashFunc _h;
    };

    template <
        typename T
        template <typename> PerfectHashFunc,

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

#endif