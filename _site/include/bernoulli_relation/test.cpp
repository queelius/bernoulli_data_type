

#include <variant>
#include <optional>
#include <tuple>
#include <string>
#include <iostream>
#include <tuple>

#include <cstdlib>

template <size_t... n>
struct ct_integers_list {
    template <size_t m>
    struct push_back
    {
        typedef ct_integers_list<n..., m> type;
    };
};

template <size_t max>
struct ct_iota_1
{
    typedef typename ct_iota_1<max-1>::type::template push_back<max>::type type;
};

template <>
struct ct_iota_1<0>
{
    typedef ct_integers_list<> type;
};

template <size_t... indices, typename Tuple>
auto tuple_subset(const Tuple& tpl, ct_integers_list<indices...>)
    -> decltype(std::make_tuple(std::get<indices>(tpl)...))
{
    return std::make_tuple(std::get<indices>(tpl)...);
    // this means:
    //   make_tuple(get<indices[0]>(tpl), get<indices[1]>(tpl), ...)
}

template <typename Head, typename... Tail>
std::tuple<Tail...> tuple_tail(const std::tuple<Head, Tail...>& tpl)
{
    return tuple_subset(tpl, typename ct_iota_1<sizeof...(Tail)>::type());
    // this means:
    //   tuple_subset<1, 2, 3, ..., sizeof...(Tail)-1>(tpl, ..)
}


struct flatten_visitor
{
    template <typename ... T>
    void operator()(std::variant<T ...> v) const
    { 
        std::cout << typeid(v).name() << std::endl;
        std::cout << "variant of variant\n"; 
        std::visit(flatten_visitor{}, v);
    }

    void operator()(double f) const
    {
        std::cout << typeid(f).name() << std::endl;
        std::cout << "vairant of float " << f << "\n"; 
    }
    void operator()(int i) const
    { 
        std::cout << typeid(i).name() << std::endl;
        std::cout << "vairant of int " << i << "\n"; 
    }

    void operator()(char c) const
    { 
        std::cout << typeid(c).name() << std::endl;
        std::cout << "vairant of char " << c << "\n"; 
    }

    void operator()(bool b) const
    { 
        std::cout << typeid(b).name() << std::endl;
        std::cout << "vairant of bool " << b << "\n"; 
    }

};


int main()
{
    using T1 = std::tuple<std::tuple<int,bool>,std::tuple<unsigned,char>>;
    //auto a = std::make_tuple(1, "hello", 7.9);
    auto b = std::get<0>(T1{});

    const char* s = nullptr;
    double d = 0.0;
    std::tie(s, d) = b;
    printf("%s %g\n", s, d);
    // prints:   hello 7.9    
    return 0;

    using X = std::variant<int, char>;
    using Y = std::variant<double, X>;
    using Z = std::variant<bool, Y>;

    X x = (int)10.2;
    Y y = x;
    Z z = y;
    Z z2 = (X)('a');
    //std::visit(flatten_visitor{}, x);
    //std::visit(flatten_visitor{}, y);
    std::visit(flatten_visitor{}, z2);


    
}