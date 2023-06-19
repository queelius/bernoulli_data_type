However, for other
types, such as integers, there are theoretically as many cases to consider as
there are possible values. If we extend that to composite types, such as arrays of
size `N`, then there are `|T|^N` cases to consider where `T` is the type of the
elements of the array. For instance, if `T = bool`, then there are `2^N` cases to
consider. If `T = char`, then there are `2^8^N = 256^N` cases to consider, and if
`T = array<X,N>`, then there are `|X|^N^N` cases to consider.
