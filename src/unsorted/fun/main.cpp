#include <iostream>
//#include <utility>
//#include "intrinsics.h"
//#include "type_functions.h"
#include <functional>

/*
template <typename F, typename G>
class ComposeUnaryFunctor :
    public std::unary_function<typename F::argument_type,
                               typename G::argument_type>

// compose(f, g) = f . g where f : Y -> Z and g : X -> Y
template <typename F, typename G>
    requires(UnaryFunction(F) && UnaryFunction(G) &&
             Domain(F) == Codomain(G))
auto compose(F&& f, G&& g)
{
    return [f=std::move(f),g=std::move(g)](auto x)
    {
        return f(g(x));
    };
};
*/

template <typename F, typename G>
auto compose(F f, G g)
{
    return [=](auto x)
    {
        return std::invoke(
            f,
            std::invoke(g,x));
    };
};

template <typename F, typename G>
auto generalized_compose(F f, G g)
{
    return [=](auto... x)
    {
        return std::invoke(
            f,
            std::invoke(g,x...)...);
    };
};


template <typename F, typename G>
auto on(F f, G g)
{
    return [=](auto... x)
    {
        return std::invoke(
            f,
            std::invoke(g,x)...);
    };
};

template <typename F, typename G>
auto partial_compose(F f, G g, int arg)
{
    return [=](auto... x)
    {

    };
};




int main()
{
    // f . g : Int -> Double
    auto fg = compose([](int y) { return y * .5; }, [](int x) { return x; });
    std::cout << fg(1) << std::endl;
}