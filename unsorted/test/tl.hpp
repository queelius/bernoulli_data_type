#include <variant>

// (variadic) type list
template <typename... Ts>
struct tl
{
    using size = std::integral_constant<std::size_t, sizeof...(Ts)>;
};

template <std::size_t idx, class... Types>
class extract
{
    static_assert( idx < sizeof...( Types ), "index out of bounds" );

    template <std::size_t i, std::size_t n, class... Rest>
    struct extract_impl;

    template <std::size_t i, std::size_t n, class T, class... Rest>
    struct extract_impl<i, n, T, Rest...>
    {
        using type = typename extract_impl<i + 1, n, Rest...>::type;
    };

    template <std::size_t n, class T, class... Rest>
    struct extract_impl<n, n, T, Rest...>
    {
        using type = T;
    };

public:
    using type = typename extract_impl<0, idx, Types...>::type;
};

template <std::size_t idx, class TypeList>
struct type_list_extract;

template <std::size_t idx, template <class...> class TypeList, class... Types>
struct type_list_extract<idx, TypeList<Types...>>
{
    using type = typename extract<idx, Types...>::type;
};

template <std::size_t idx, class TypeList>
using type_list_extract_t = typename type_list_extract<idx, TypeList>::type;

// prepend a type to the (variadic) type list
template <typename TList, typename T>
struct prepend;

template <typename ... Ts, typename T>
struct prepend<T, tl<Ts...>>
{
    using type = tl<T, Ts...>;
};

template <typename TList, typename T>
using prepend_t = typename prepend<T, TList>::type;

template <typename ... Ts1, typename ... Ts2>
struct prepend<tl<Ts1...>, tl<Ts2...>>
{
    using type = tl<Ts1..., Ts2...>;
};

template <typename TList, typename T>
struct append;

template <typename ... Ts, typename T>
struct append<T, tl<Ts...>>
{
    using type = tl<Ts..., T>;
};

template <typename TList, typename T>
using append_t = typename append<T, TList>::type;

template <typename ... Ts1, typename ... Ts2>
struct append<tl<Ts1...>, tl<Ts2...>>
{
    using type = tl<Ts2..., Ts1...>;
};

template<typename... Ts> 
std::variant<Ts...> as_variant(tl<Ts...>);

template<typename... Ts> 
std::tuple<Ts...> as_tuple(tl<Ts...>);

template<typename... Ts> 
tl<Ts...> as_type_list(std::variant<Ts...>);

template<typename... Ts> 
tl<Ts...> as_type_list(std::tuple<Ts...>);

template<typename... Ts> 
tl<Ts...> as_type_list(Ts...);

