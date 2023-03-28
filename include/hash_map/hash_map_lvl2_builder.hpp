        // I models a forward iterator of (x,y) pairs.
        template <typename I>
        hash_map_lvl2(
            I begin,
            I end,
            D d,
            H h,
            double r) :
                h_(h)
                d_(d),
                hs_(static_cast<size_t>(std::ceil(std::distance(begin,end)/r)))
        {
            using std::vector;
            using std::pair;

            auto const m = std::distance(begin,end);
            size_t min_block_errors = m;
            for (size_t s0 = 1; s0 != 0; ++s0)
            {
                vector<vector<pair<size_t,output_type>>> parts(nbins());
                for (auto xy = begin; xy != end; ++xy)
                {
                    auto s = s0_;
                    auto hx = h_(xy.first);
                    hash_combine(s, hx);
                    auto bin = s % nbins();
                    parts[bin].push_back(make_pair(hx,xy.second));
                }

                for (size_t block = 0; block < parts.size(); ++block)
                {
                    size_t block_errors = 0;
                    for (size_t bs = 1; bs != 0; ++bs)
                    {
                        size_t block_bs_errors = 0;
                        for (auto [hx,y] : parts[block])
                        {
                            auto s = s0_;
                            hash_combine(s,hx);
                            hash_combine(s,bs);
                            if (d_(s) != y)
                                ++block_bs_errors;
                        }

                        if (block_bs_errors > block_errors)
                        {
                            max_correct = correct;
                            s0_ = s0;

                        }
                    }
                }
            }
        }
