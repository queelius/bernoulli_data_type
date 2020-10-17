#include <iostream>

template<typename T>
requires true || false  // True concept
struct Foo
{};

template<typename T>
requires false || true // False concept
struct Bar
{};


template <typename T>
class UniversalSet
{
public:
    using value_type = T;

    bool contains(T const& x) { return true; };
};

template <typename T>
class EmptySet
{
public:
    using value_type = T;

    bool contains(T const& x) { return false; };
};


template <typename T>
class PerfectHashFilter
{
public:

    bool contains(T const& x) { return true; };
};

int main()
{
    Foo<int>(); // Ok
    Bar<int>(); // Fail
    return 0;
}
