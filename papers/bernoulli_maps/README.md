# Random approximate map model

Random approximate maps are defined by two sources of rate distrotion, rate distrotion of the domain, principally the domain of definition, and rate distortion of images that are in the domain of definition.

## Domain rate-distortion.
Let f : X \-> Y be a partial map from domain X to
codomain Y.

A random approximate partial map f[tpr,fpr] : X \-> Y
is a probabilistic map satisfying the following
probabilistic axiom on the positive domain of
definition.

Axiom 1: Rate distorted positive domain of definition
Elements not in the domain of definition of f are
in the domain of definition of f[tpr,fpr] with
a probability rate fpr.

Generally, although this may be relaxed, the values
not in the domain of definition of f are mapped to
random values in the codomain Y by f[tpr,fpr].

However, it may instead map to values in the
codomain that have a more likely preimage. If
f[tpr,fpr] is suppose to be a cipher map, this is
problematic since it stores information about
the distribution of X.

In some implementations, it may not even follow
this relaxed model, and instead (for reasons of
efficiency) preferentially or exclusively only map
to elements in the codomain that are mapped to by
f, e.g., a AHM (approximate hash map) with a
maximum load factor 1 and no indirection.
     
## Image rate-distortion.
Let f : X \-> Y be a partial map from domain X to
codomain Y.

A random approximate partial map f[tpr,fpr] : X \-> Y
is a probabilistic map satisfying the following
probabilistic axiom.

Axiom 2:
Elements in the domain of definition of f are not in the
domain of definition of f[tpr,fpr] with a probability
rate tpr.

Axiom 3:
If f(x) = y, x in DoD(f[tpr,fpr]) with probability tpr
as before. However, if x in DoD(f[tpr,fpr]), then there
is a rate distrotion that may be applied to the value
y.
