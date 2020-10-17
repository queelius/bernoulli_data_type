#include <iterator>
#include <iostream>
#include <vector>

template <typename I, typename Cmp> 
void merge(I begin, I pivot, I end, Cmp cmp);

template <
    typename I,     // I models forward iterator
    typename Cmp    // Cmp models binary predicate
> 
void mergesort(I begin, I end, Cmp cmp)
{
    const auto N = std::distance(begin, end);
    if (N < 2)
        return;
    
    auto mid = begin;
    std::advance(mid, N / 2);

    mergesort(begin, mid, cmp);
    mergesort(mid, end, cmp);

    merge(begin, mid, end, cmp);
}

template <
    typename I,     // I models forward iterator
    typename Cmp    // Cmp models binary predicate
> 
// [begin, pivot) and [pivot, end) are totally sorted
// ranges (by cmp) and [begin, end) is a range.
/*
void merge(I begin, I pivot, I end, Cmp cmp)
{
    std::vector<int> xs;

    auto i = begin,
         j = pivot;

    while (i != pivot && j != end)
    {
        if (cmp(*i,*j))
        {
            xs.push_back(*i);
            ++i;
        }
        else
        {
            xs.push_back(*j);
            ++j;
        }
    }
    while (i != pivot)
        xs.push_back(*i++);
    while (j != end)
        xs.push_back(*j++);

    auto k = xs.begin();
    while (begin != end)
    {
        *begin = *k++;
        ++begin;
    }
}
*/


void merge(I begin, I pivot, I end, Cmp cmp)
{
    auto i = begin,
         j = pivot;

    while (i != pivot && j != end)
    {
        if (cmp(*i,*j))
            ++i;
        else
        {
            std::swap(*i,*j);
            ++j; ++i;
        }
    }
}


int main()
{
    std::vector<int> data{3, 1, 9, 0, 6, 5, 4, 7, 2, 8};
    //std::vector<int> data{1, 2, 3, 4, 7, 8, 5, 6};

    mergesort(std::begin(data), end(data), [](int x, int y) { return x < y; });

    for (auto x : data)
        std::cout << x << '\n';

}