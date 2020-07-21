/**
 * Suppose we have some type constructor
 *     + : Type -> Type -> Type
 * such that A+B is a type that has as values the disjoint union
 * of the elements in A and B.
 * 
 * Suppose we have a function not : {0} + {1} -> {0} + {1}
 * defined as
 * 
 *     not 0 := 1
 *     not 1 := 0
 * 
 * Suppose we have an prefix-free coder for {0}+{1} given by
 * 
 *     enc(0) := {11}
 *     enc(0) := {0,10},
 * 
 * which are optimal codes if the probability masses
 * are
 * 
 *     P(0) = 1/4 
 *     P(1) = 3/4.
 *  
 * Suppose we have a first-order random approximate type
 * of {0}+{1}, where
 * 
 *     
 * 
 *     P[not(1) == 0] = 1-r
 *     P[not(1) == 1] = r
 * 
 * where r is some rate distortion or noise.
 * 
 */
