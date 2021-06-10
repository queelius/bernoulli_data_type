# Types

We say that a tick, `'`, followed by a sequence of alphabetic symbols is an `Atom`, i.e., `'a` and `'b` are `Atoms` but `a` is not.

We say that the *type* of `'a` is an `Atom`.

Suppose we say that a `Pair` begins with `cons` and ends with two more parts, its `car` and its `cdr`. So, `cons 'a 'b` is a `(Pair Atom Atom)` (that is its type) and `cdr (cons 'a 'b)` is the same `Atom` as `'b`.

Sentences like "`'a` is an `Atom`", "`cons 'a 'b'` is a `(Pair Atom Atom)` are judgements -- that is, attitudes that people take towards expressions.

Another judgement is "`cons 'a 'b` is the same `(Pair Atom Atom)` as `cons 'a b'`". However, `cons 'a 'b` is not the same `Atom` as `cons 'b 'a`. For two `(Pair Atom Atom)` to be the same, they must have the same `car` and `cdr`.

It must also be true that `cons 'a 'b'` is the same `(Pair Atom Atom)` as `car (cons (cons 'a 'b) 'c)`, but `cons 'a 'b'` is a simpler expression. Given the set of expressions that are the same `Atoms` as `'a`, the *simplest* expression is its normal form, which is just `'a`.

## Normal Forms

Definition: Given a type, every expression described by that type has a normal form, which is the most direct way of writing it. If two expressions are the same, then they have identical normal forms, and if they have identical normal forms, then they are the same.

In a random approximate set over a countably infinite universe, representational equality implies equality and equality implies representational equality. This is true even when we take unions, intersections, and complements of random approximate sets, i.e., two such compositions are equal iff, when we reduce both compositions to, say, disjunctive normal form, they are the same if they have the same disjunctive normal form and each term (random approximate set) has the same representation.

This has the interesting property that the normal form is provided by the disjunctive (or conjunctive) normal form over the random approximate sets with some canonical ordering, e.g., lexographic order on the serialization of the composed random approximate infinite sets.

For finite universal sets, the normal form of a random approximate set is just the elements in the set, since we can enumerate each of them.

If enumerate is not practical, then we may reduce them to disjunctive normal form and do the equality comparisons on each of the operations and terms. However, in this case, while representational equality implies equality, equality does not imply representational equality since there are many ways to represent the same set.

In fact, there are many ways to represent the same infinite set too, but since the nature of the random approximate set is presumably random, ...




There are four basic forms of judgement:

1. _ is a _.
2. _ is the same _ as _.
3. _ is a type.
4. _ and _ are the same type.

There may be many ways to say the same thing.

