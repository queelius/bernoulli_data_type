#include <iostream>
#include <string_view>

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

struct X { };

std::ostream& operator<<(std::ostream& os, X x)
{
    std::cout << "X" << std::endl;
    return os;
};


bool operator<(X, X)
{
    return true;
}

void print(const std::string_view& x)
{
    std::cout << x << std::endl;
}

void* operator new(std::size_t n)
{
    std::cout << "[allocating " << n << " bytes]\n";
    return malloc(n);
}

double my_sqrt(double x);

double my_sqrt(double);

float my_sqrt(float y);


template <typename T>
T my_sqrt(T x);

int main()
{
    print(std::string("testasdfjakhjakfhadhjasfkljasdkljakl;sfjaskl;fjdaskl;"));
    print("testasdfjakhjakfhadhjasfkljasdkljakl;sfjaskl;fjdaskl;");

    my_sqrt(4.);
}

double my_sqrt(double z)
{
    return z;
}

double my_sqrt(double adsfasfdy);