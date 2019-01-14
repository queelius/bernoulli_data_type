#include <vector>

namespace alex::math
{
    template <class Y>
    struct interval_arithmetic
    {
        interval_arithmetic<Y> operator+(const interval_arithmetic<Y>& lhs)
        {
            return interval_arithmetic<Y>{ min() + lhs.min(), max() + lhs.max() };
        };
    };

    template <
        class T = double,
        template <class> class OperatorSet = interval_arithmetic>
    class interval: public interval_arithmetic<interval<T, OperatorSet>>
    {
    public:


    };

/*
    template <
        class T,
        template <class> class OperatorSet,
        template <class> class Container,
        template <class> class Interval>
    class disjoint_interval_set;

    template <class T>
    struct disjoint_interval_arithmetic
    {
        template <class U,
        disjoint_interval_arithmetic operator+(const U& lhs)
        {

        };
    };

    template <
        class T,
        template <class> class OperatorSet = disjoint_interval_arithmetic,
        template <class> class Container = std::vector,
        template <class> class Interval = interval
    >
    class disjoint_interval_set:
        public Container<T>,
        public OperatorSet<T>
    {

    };
    */
}