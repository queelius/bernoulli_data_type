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

template <typename T>
struct is_ranged
{
private:
	template<typename U> static auto test(int) ->
		decltype(declval<U>().min() == 1 && declval<U>().max() == 1, true_type());

	template<typename> static false_type test(...);

public:
	static constexpr bool value = is_same<decltype(test<T>(0)), true_type>::value;
};

template <typename T>
concept bool HasMinimum = requires(T x)
{
    { x.min() };
};

template <typename T>
concept bool HasMaximum = requires(T x)
{
    { x.max() };
};

template <class T>
concept Real = std::is_float<T>::value;

template <typename T>
concept bool IsRanged = && HasMinimum<T> && HasMaximum<T>;

