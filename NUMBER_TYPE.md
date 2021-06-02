# Approximate number types

Assuming $T$ models a numerical type $R$ where $T$ has
$n$ digits of precision and $R$ has $N$ digits of precision,
$n < N$, then

$$
    == : (T,T) -> bool
$$

is an *approximation* of

$$
    == : (R,R) -> bool
$$

since, the mapping from $R$ to $T$ must be non-injective, i.e.,
at least two values in $R$ map to the same value in $T$.

Depending on how $T$ represents $R$, there may be variability
in the approximation error over the values in $R$.

Suppose $R$ represents numbers as a sequence of $N$ bits, where
the $k$-th most significant bit is the $k$-th element of the sequence.

Then, if $T$ models numbers in the same way, except with $n$ bit
sequences where $n < N$, then the mapping from $R$ to $T$ that results
in the smallest uniform error is given by keeping the first $n$ bits
of the sequence.

If we do this, the error is bounded by

$$
    [0,2^(N-n)].
$$

If we only wish to view this as a Bernoulli approximation, then

$$
    P[T(X) == R(X) | X ~ UNIF(R)] = 2^-(N-n)
$$

where $UNIF(R)$ is the discrete uniform distribution over $R$.

If $R$ is an infinite set, like the real numbers, then $T$, as a
finite precision representation, can only exactly model a finite number
of numbers on the order of $O(2^n)$ and the probability $P[T(X) == R(X)]$
is zero unless we assign non-zero probability to specific values of $X$
in the domain of $T$.
