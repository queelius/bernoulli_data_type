



Suppose we have a sequence of $n$ elements.

If we want to determine if the sequence is sorted, we may do a single pass
through the sequence, checking that $x_i <= x_{i+1}$ for $i=1,...,n-1$.

However, this may be slower than is acceptable for whatever reason.
Alternatively, we could check that specific values within the sequence are in
sorted order with respect to each other.

Assume the sequence contains values sampled uniformly from a value type $X$.

Say we have a sequence of $3$ Boolean values, $0$ or $1$, and we check the
first two values for sortedness. What is the probability the sequence is sorted
if we assume the Boolean values are sampled uniformly at random?

There are $2^n$ sequences.

There are $8$ possible sequences. The number of sequences that start with the
first two positions sorted