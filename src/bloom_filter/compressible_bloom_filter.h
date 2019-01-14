#ifndef __COMPRESSIBLE_BLOOM_FILTER_H__
#define __COMPRESSIBLE_BLOOM_FILTER_H__

#include "bloom_filter.h"

namespace alex { namespace collections
{
    class CompressibleBloomFilter : public BloomFilter
    {
    public:
        CompressibleBloomFilter(const BloomFilterParameters& p)
            : BloomFilter(p)
        {
            size_list.push_back(table_size_);
        }

        unsigned long long int size() const
        {
            return size_list.back();
        }

        bool compress(const double percentage)
        {
            if ((0. >= percentage) || (percentage >= 100.))
            {
                return false;
            }

            unsigned long long int original_table_size = size_list.back();
            unsigned long long int new_table_size = static_cast<unsigned long long int>((size_list.back() * (1. - (percentage / 100.))));
            new_table_size -= (((new_table_size % bits_per_char) != 0) ? (new_table_size % bits_per_char) : 0);

            if ((bits_per_char > new_table_size) || (new_table_size >= original_table_size))
            {
                return false;
            }

            desired_false_positive_probability_ = effective_fpp();
            cell_type* tmp = new cell_type[static_cast<size_t>(new_table_size / bits_per_char)];
            std::copy(bit_table_, bit_table_ + (new_table_size / bits_per_char), tmp);
            cell_type* itr = bit_table_ + (new_table_size / bits_per_char);
            cell_type* end = bit_table_ + (original_table_size / bits_per_char);
            cell_type* itr_tmp = tmp;

            while (end != itr)
            {
                *(itr_tmp++) |= (*itr++);
            }

            delete[] bit_table_;
            bit_table_ = tmp;
            size_list.push_back(new_table_size);

            return true;
        }

     private:
        void compute_indices(const bloom_type& hash, size_t& bit_index, size_t& bit) const
        {
            bit_index = hash;
            for (size_t i = 0; i < size_list.size(); ++i)
            {
                bit_index %= size_list[i];
            }
            bit = bit_index % bits_per_char;
        }

        std::vector<unsigned long long int> size_list;
    };
}}

#endif