
# Bernoulli algorithms

Return a random element with a false positive rate
    k / N,
where k is size of black list set and N is cardinality of domain.

Randomly sample without replacement m of the k elements
and returning the first sample with no collisions.

The false positive rate is r = (k - m)/N, and the no-collision
outcome is geometrically distributed with probability of no
collision 1-r and thus there are expected to be 1/(1-r) trials.

## Bernoulli algorithm: is_sorted : bool* -> bernoulli<?,bool>



Suppose we have a sequence of $n$ elements.

If we want to determine if the sequence is sorted, we may do a single pass
through the sequence, checking that $x_i \leq x_{i+1}$ for $i=1,...,n-1$.

However, $O(n)$ may be too slow for whatever reason.
Alternatively, we could *sample* (without replacement) a subset and determine if
the subset is sorted and use that information to estimate, with some probability
of error, that the larger collection is sorted.

Assume the sequence contains values sampled uniformly from a value type $X$.

Say we have a sequence of $3$ Boolean values, $0$ or $1$, and we check the
first two values for sortedness. What is the probability the sequence is sorted
if we assume the Boolean values are sampled uniformly at random?

There are $2^n = 2^4 = 16$ possible sequences over Boolean values.

$$
    \{
        (0, 0, 0, 0),
        (0, 0, 0, 1),
        (0, 0, 1, 0),
        (0, 0, 1, 1),\\
        (0, 1, 0, 0),
        (0, 1, 0, 1),
        (0, 1, 1, 0),
        (0, 1, 1, 1),\\
        (1, 0, 0, 0),
        (1, 0, 0, 1),
        (1, 0, 1, 0),
        (1, 0, 1, 1),\\
        (1, 1, 0, 0),
        (1, 1, 0, 1),
        (1, 1, 1, 0),
        (1, 1, 1, 1)
    \}.
$$


Of these, those that are in order, assuming $0 < 1$, are given by

$$
    \{
        (0, 0, 0, 0),
        (0, 0, 0, 1),
        (0, 0, 1, 1),\\
        (0, 1, 1, 1),
        (1, 1, 1, 1)
    \}.
$$


### Case m=1
If we sample $m=1$, this is *always* sorted. Recall that we assume that the
collection has elements sampled uniformly from $X := \rm{Boolean}$. So, what is
the probability that the larger collection is ordered?

If we sampled a $0$, we look at all the ordered sub-samples that contain $0$ and
then divide it by the total number of sub-samples that contain $0$.
There are $4$ ordered sub-samples that contain $0$ and there are $15$ sub-samples
that contain $0$, thus the probability that the collection is sorted is given
by $4/15$.

If we sampled a $1$, then we see there are $4$ ordered sub-samples that contain
$1$ and $15$ sub-samples that contain $1$. Again, the probability comes out to
$4/15$.

In each case, the probability that $\rm{is_sorted}$ outputs an erroneous value
is $11/15$.
This calls for a larger sample.

### Case m=2
If we sample $m=2$, what is the probability that the larger collection is ordered
if the sample is ordered? The possible ordered sub-samples are given by

$$
    \{
        (0,0),(0,1),(1,1)
    \}.
$$

If we sampled a $(0,0)$, we look at all the ordered sub-samples that contain
$(0,0)$ and then divide it by the total number of sub-samples that contain $(0,0)$.
There are $3$ ordered sub-samples that contain $(0,0)$ and there are $8$ sub-samples
that contain $(0,0)$, thus the probability that the collection is sorted is given
by $3/8$.

If we sampled a $(0,1)$, then we see there are $3$ ordered sub-samples that contain
$(0,1)$ and $11$ sub-samples that contain $(0,1)$, thus the probability that the collection is sorted is given by $3/11$.

If we sampled a $(1,1)$, then we see there are $3$ ordered sub-samples that contain
$(1,1)$ and $8$ sub-samples that contain $(1,1)$, thus the probability that the collection is sorted is given by $3/8$.

The probability of error is still probably too large.

### Case m=3
If we sample $m=3$, what is the probability that the larger collection is ordered
if the sample is ordered? The possible ordered sub-samples are given by

$$
    \{
        (0,0,0),(0,0,1),(0,1,1),(1,1,1)
    \}.
$$

If we sampled a $(0,0,0)$, we look at all the ordered sub-samples that contain
$(0,0,0)$ and then divide it by the total number of sub-samples that contain $(0,0,0)$.
There are $2$ ordered sub-samples that contain $(0,0,0)$ and there are $3$ sub-samples
that contain $(0,0,0)$, thus the probability that the collection is sorted is given
by $2/3$.

If we sampled a $(0,0,1)$, then we see there are $2$ ordered sub-samples that contain
$(0,0,1)$ and $4$ sub-samples that contain $(0,0,1)$, thus the probability that the collection is sorted is given by $2/4=1/2$.

If we sampled a $(0,1,1)$, then we see there are $2$ ordered sub-samples that contain
$(1,1)$ and $4$ sub-samples that contain $(0,1,1)$, thus the probability that the collection is sorted is given by $3/8$.

If we sampled a $(1,1,1)$, then we see there are $2$ ordered sub-samples that contain
$(1,1,1)$ and $3$ sub-samples that contain $(1,1,1)$, thus the probability that the collection is sorted is given by $2/3$.

The probability of error is still probably too large.
However, if we sample $m=4$, the probability of error is $0$ since this reduces
to the exact $\rm{is_ordered}$ test function.

