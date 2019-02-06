template <typename T>
concept bool Addable = requires(T x)
{
    { x + x };
};

template <typename T>
concept bool EqualityComparable = requires(T x)
{
    { x == x } -> bool;
};

template <typename T>
concept bool LessThanComparable = requires(T x)
{
    { x < x } -> bool;
};

template <typename T> requires LessThanComparable<T>
T max(T a, T b)
{
    return b < a ? a : b;
};
