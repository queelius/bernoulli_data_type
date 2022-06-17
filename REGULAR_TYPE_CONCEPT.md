Concept: RegularType {#regular_type}
====================================

A `RegularType` is a concept that requires the following. Consider an object type `X`. If `X` is a `RegularType`, then at minimum it implements assignment, default constructor, copy constructor, and equality comparison.

C++ regular types
-----------------

In C++, a class `X` (object type) that is a `RegularType` if it has the following operations (overload set):

1. Assignment: `bool operator=(const X&)` must be implemented.
2. Default constructor: `X::X()` must be implemented.
3. Copy constructor: `X::X(const X&)` must be implemented.
4. Equality: `bool operator==(const X&)` must be implemented.

For example, the following code snippet must compile successfully.

~~~ cpp
    X x;                        // default constructor
    X y(x);                     // copy constructor
    std::static_assert(y == x); // equality comparison
    y = x;                      // assignment
~~~
