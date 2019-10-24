#include <iostream>
#include <variant>
#include <type_traits>
#include <tuple>
#include <string>

// (variadic) type list
template <typename... Ts>
struct tl
{
    using size = std::integral_constant<std::size_t, sizeof...(Ts)>;
};

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
std::variant<Ts...> as_variant(tl<tl<Ts...>>);

template<typename... Ts> 
std::tuple<Ts...> as_tuple(tl<Ts...>);

template<typename... Ts> 
std::tuple<Ts...> as_tuple(tl<tl<Ts...>>);

template<typename... Ts> 
tl<Ts...> as_type_list(std::variant<Ts...>);

template<typename... Ts> 
tl<Ts...> as_type_list(std::variant<tl<Ts...>>);

template<typename... Ts> 
tl<Ts...> as_type_list(std::tuple<Ts...>);

template<typename... Ts> 
tl<Ts...> as_type_list(std::tuple<tl<Ts...>>);

template<typename... Ts> 
tl<Ts...> as_type_list(Ts...);

template <typename T>
struct to_type_list
{
    using type = decltype(as_type_list((T){}));
};

template <typename L, typename R>
struct sum_variant
{
    using type_list_left = typename to_type_list<L>::type;
    using type_list_right = typename to_type_list<R>::type;
    using type_list = append_t<type_list_left,type_list_right>;
    using left_type = decltype(as_variant(type_list_left{}));
    using right_type = decltype(as_variant(type_list_right{}));
    using type = decltype(as_variant(type_list{}));
};


struct VisitPackage
{
    void operator()(char) { std::cout << "char\n"; }
    void operator()(int) { std::cout << "int\n"; }
    void operator()(std::string) { std::cout << "string\n"; }
    void operator()(unsigned) { std::cout << "unsigned\n"; }
};


template <typename Variant>
void test(Variant x)
{
    std::visit(VisitPackage(), x);
};

int main()
{
    using L = std::variant<int,char>;
    using R = std::variant<std::string,unsigned>;
    using S = sum_variant<L,R>;
    using LR = S::type;

    LR myint = 2;
    LR myunsigned = 3u;
    LR mystring = std::string("test");
    LR mychar = 'a';

    test(myint);
    test(myunsigned);
    test(mychar);
    test(mystring);

    using XX = S::type_list_left;

    //std::cout << std::holds_alternative<S::left_type>(myint) << std::endl;
    std::cout << std::holds_alternative<S::type_list_left>(myint) << std::endl;
}

