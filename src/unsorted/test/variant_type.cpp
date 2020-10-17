#include <iostream>
#include <vector>
#include <any>


class object_t
{
public:
    //template <typename T>
    //object_t(T x) : self_()
};


void testany()
{
    std::any x = 3;
    x.
}


struct Type
{
    virtual int type() { return 0; };
};

struct IntType final : virtual Type
{
    IntType(int value) : value(value) {};
    int value;
};

struct CharType final: virtual Type
{
    CharType(char value) : value(value) {};
    char value;
};

struct BoolType final: virtual Type
{
    BoolType(bool value) : value(value) {};
    bool value;
};

template <typename T1, typename T2>
struct CartesianType final: virtual Type
{
    CartesianType(Type* left, Type* right) :
        left(dynamic_cast<T1*>(left)), right(dynamic_cast<T2*>(right)) {};
    T1* left;
    T2* right;
};

template <typename T1, typename T2>
struct SumType final: virtual Type
{
    SumType(Type* val)
    {
    }

    T1* left() { return dynamic_cast<T1*>(val); };
    T2* right() { return dynamic_cast<T2*>(val); };

    Type* val;
};


template <typename T>
struct Set
{
    using type = T*;

    void insert(type x)
    {
        values.push_back(x);
    };

    bool contains(type x) const
    {
        for (auto v : values)
        {
            if (x->value == v->value) return true;
        }
        return false;
    };

    std::vector<type> values;

    std::string label;
};

int main()
{
    Type* value = new IntType(1);
    Type* value2 = new BoolType(1);
    auto pairval = new CartesianType<IntType,BoolType>(value,value2);


    Set<CartesianType<IntType,BoolType>> s;
    std::cout << s.contains(pairval) << std::endl;

    if (dynamic_cast<CharType*>(value))
    {
        std::cout << "It's a char type!\n";

        std::cout << dynamic_cast<CharType*>(value)->value << std::endl;
    }
    if (dynamic_cast<IntType*>(value))
    {
        std::cout << "It's an int type!\n";

        std::cout << dynamic_cast<IntType*>(value)->value << std::endl;
    }

}