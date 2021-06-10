Perfect hash filter (PHF)
=========================

The perfect hash filter (PHF) is a rate-distorted (frozen) set that models
the second-order Bernoulli type over 2^X.

It is implemented by composing a perfect hash function, or a rate-distorted
perfect hash function, a vector of hashes, a bit matrix, and a codec.
