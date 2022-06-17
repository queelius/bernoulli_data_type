Concept: Codec {#codec}
=======================

A *datum* is a sequence of bits. A *value* is a sequence of bits and its interpretation. A *value type* is a set of values sharing a common interpretation.

We denote the datum assigned to a value `x` by `x'`. We say that `x` is *coded* by `x'`. Inversely, we say that `x'` has an *decoding* (interpretation) `x`. A datum without an *interpretation* has no meaning.

For example, *characters* are a fundamental *value type*. Suppose we have a set of three characters, `{a,b,c}` with codes given by the following table.

| character | code  |
|-----------|-------|
| `a`       | `0`   |
| `b`       | `10`  |
| `c`       | `110` |

In the above table, `c` and `110` are associated, i.e., `c` is coded as `c' = 110` and `c'` is decoded as `c`.

> **remark**: The codes in the above table are *prefix-free* codes. A sequence characters with prefix free codes may be unambiguously coded by simply *concatenating* the invidiaul codes, e.g., `(a b c)' <-> 010110`.

The interpretation of a value type is dictated by a set of operations. In the case of *characters*, we snuck in a lot of common understandings, e.g., the common understanding that the set of characters typically has an agreed upon *order*, i.e., alphabetic order.

To be explicit, we must also define these operations. For instance, using C++ notation, ordered sets have a less-than binary operator denoted by `<(char,char) -> {0,1}`. Given the way the characters are coded in the above table, the less-than operator has a definition equivalent to `<(x,y) := <(|x'|, |y'|)`, where `|x'|` and `|y'|` are integer types with the common understanding of the less-than operator on integer types.

Binary operators apply to the Cartesian of the set of values. We may thus represent `<(char, char) -> {0,1}` with the equivalent definition given by the following table.

| `(char, char)` | `<(char,char)` |
| -------------- | -------------- |
| `(a,a)`        | `0`            |
| `(a,b)`        | `1`            |
| `(a,c)`        | `1`            |
| `(b,a)`        | `0`            |
| `(b,b)`        | `0`            |
| `(b,c)`        | `1`            |
| `(c,a)`        | `0`            |
| `(c,b)`        | `0`            |
| `(c,c)`        | `0`            |

Codecs
------

The datums of a value type cannot exist without a computer (physical system) storing them in memory. The way a value type is implemented on a computer is denoted an *object type*.

A concept is a named set of requirements. Object type `X` implements the `Codec` concept for an object type `Y` if the following requirements are satisfied:

1. `X` is a [RegularType](./REGULAR_TYPE_CONCEPT.md) .

2. `X` must have a public member type `value_type`, the type of the coded values, i.e., `X::value_type`.

3. `X` must have an public member type `size_type` that is an `Integer` type and has a maximum value large enough to represent any coded value of type `value_type`.

4. If objects of type `value_type` has a maximum encoding size, then it may have a public member function named `maximum_bit_length` that returns the maximum bit length of a coded value of type `value_type`.

5. `X` must have a public member function compatible with the signature

~~~ cpp
template <OutputIterator O>
size_type X::encode(value_type, O);
~~~

that encodes a value of type `value_type` to the output iterator and returns the number of bits written. Typically, the encode function returns a value of type `size_type` to indicate how many bits were written to the output iterator, but
it may return something else, like `optional<size_type>`, which evaluates to a `size_type` if successful and otherwise to `null`.

6. `X` must have the member function with signature

~~~ cpp
template <InputIterator I>
optional<value_type> decode(I);
~~~

which decodes an encoded value of type `value_type` from the bits in an input iterator `I`.
