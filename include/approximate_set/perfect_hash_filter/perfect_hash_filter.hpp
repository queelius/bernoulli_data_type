#ifndef __PHFILTER_H__
#define __PHFILTER_H__

//#include <bit_matrix.hpp>

namespace alex::approximate_sets
{
    // Perfect hash filter (PHF): an implementation of the positive approximate
    // set ADT.

    template <
        typename T,
        template <class> class PerfectHashFunc,
        class HashFunc,
        class Coder,
        class BitMatrix>    
    class PerfectHashFilter
    {
        public:
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
                // if x is a member of the set, then
                // 
                auto perf_index = _perf_hash_func(x);
                auto hash_value = _hash(x) % max_hash();
                return true;
            };

            uint32_t max_hash() const { return 1 << hash_width(); };

            uint32_t hash_width() const { return _h.columns(); };

            float load_factor() const
            {
                return 0f;
            };

            double fpr() const { return std::pow(2., -hash_width()); };

            double tpr() const { return 1.; };

        private:
            static const unsigned int MAX_BYTE = (1 << CHAR_BIT);
        
            PerfectHashFunc _perf_hash_func;
            BitMatrix _hash_values;
            HashFunc _hash_func;
    };
}

#endif