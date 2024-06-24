# The Bernoulli Model: A Probabilistic Framework for Data Structures and Types

## Abstract

This paper introduces the Bernoulli Model, a probabilistic framework designed to handle uncertainty in data types, particularly Boolean values. The Bernoulli Model provides a formalism for optimizing space and accuracy in data representation through controlled trade-offs. We explore its applications in developing space-efficient data structures, such as Bloom filters and Count-Min sketches, and introduce the concept of Oblivious Data Types. The framework allows for $\mathcal{O}(1)$ time complexity while maintaining probabilistic guarantees on accuracy. This work forms a foundational part of a larger project aimed at enhancing data structure efficiency and error propagation analysis.

## Motivation

The **Bernoulli Model** is a general framework for thinking about
probabilistic data structures and types of a particular sort. 
A big reason for developing the Bernoulli Model formalism is so that we can use
Bernoulli Models of data types to develop Oblivious Data Types. We will
go into that in a separate document, but the basic idea is that Bernoulli
approximations have a lot of desirable properties for developing
oblivious data types, and the Bernoulli Model formalism allows us to reason about
the correctness of the oblivious data types and to make them more space-efficient
by trading accuracy for space while allowing for $\mathcal{O}(1)$ time complexity.

The Bernoulli Model also provides a formalism for how to think about various
probabilistic data structures, like the Bloom filter, Count-Min sketch, or my
invention, the Bernoulli data type, which comprises an entire family of data
structures that are all based on the Bernoulli Model, from sets (like the Bloom
filter) to maps in a near-space optimal way, while allowing for more savings
by trading accuracy for space in a controlled way.

## Introduction: Bernoulli Boolean

The Boolean type, denoted by $\mathrm{bool}$, models the set of values
given by $\\{\mathrm{true},\mathrm{false}\\}$, or more compactly $\\{0,1\\}$.
This document entertains the replacement of $\mathrm{bool}$ with a
type $B_{\mathrm{bool}}$, which represents a sort of *noisy* Boolean.
In general, we can have a Bernoulli type for any type $T$, denoted by $B_T$.

> As special case, data structures like Bloom filters can be thought of as a
> Bernoulli type for the set indicator function, $1_A : X \mapsto \{0,1\}$,
> but more on that later.

There are two types that have fewer values than the Boolean type, the absurd type, denoted by $\mathrm{void}$, which is the type with no values (and thus values of this type cannot be constructed) and the unit type, denoted by $()$, which has only a single value, also denoted by $()$. Since there is only a single value of the unit type, there is no uncertainty in the unit type.

As degenerate cases, the Bernoulli Model of the absurd is just the absurd type,
$B_{\mathrm{void}} \equiv \mathrm{void}$, and Bernoulli Model of the unit type is just the unit type, $B_{()} \equiv ()$.

The Boolean type $\mathrm{bool}$ has two possible values, $\mathrm{true}$ and $\mathrm{false}$,
and is thus the first type for which we can introduce uncertainty.

Every Bernoulli Model also has an *order*, an integer greater than 1, which essentially describes the number of independent ways in which the process that generated the Bernoulli approximation can produce errors. We denote that a Bernoulli Model of type $T$ has order $k$ with $B_T^{(k)}$. For the absurd and unit types, the maximum order is $0$, and in general $T \equiv B_T^{(0)}$, i.e., if there are no ways to introduce errors, then that is equivalent to the type itself.

Unless it is useful, we drop the order information and simply write $B_T$. In the Bernoulli Model, we may denote the latent value $x$ that is being approximated using the notation $B_T(x)$ and we say that it is latent if we do not know the value of $x$ and we are trying to approximate it using the Bernoulli Model approximation $B_T(x)$. In this case, the latent value $x$ is unobservable and we can think of $B_T(x)$ as a noisy measurement of $x$.

In the Bernoulli Boolean Model, $B_{\mathrm{bool}}(x)$ is a random Bernoulli variable such that
$$
\Pr\\{ B_{\mathrm{bool}}(x) \neq x\\} = \varepsilon(x)
$$
for each $x \in \\{0,1\\}$, where $0 < \varepsilon(x) < 1$ is the probability of an error. In most practical situations, $\varepsilon(x)$ is known or its expectation is known, and it can be pre-specified to balance factors like space complexity and accuracy.

## Binary Channels

Let's begin by thinking about the Binary Symmetric Channel and the Binary
Asymmetric Channel. The Bernoulli Boolean model can exhibit two distinct
behaviors, represented as different "channels" through which Boolean values are
transmitted:

1. **Binary Symmetric Channel (First-order Bernoulli Model)**: The probability of an
equality error is the same for $1$ and $0$. We denote this by the type
$B_{\mathrm{bool}}^{(1)}$.

2. **Binary Asymmetric Channel (Second-order Bernoulli Model)**: The probability of
an equality error differs for $1$ and $0$. We denote this by the type
$B_{\mathrm{bool}}^{(2)}$.

## False Positives and Negatives

Errors in the Bernoulli Boolean model can be understood in terms of
*false negatives* and *false positives*:

1. $B_{\mathrm{bool}}(0) = 1$ is a *false negative*.
2. $B_{\mathrm{bool}}(1) = 0$ is a *false positive*.

In the first-order model, the probability of a false negative equals the probability
of a false positive. In the second-order model, these probabilities differ.
In a specific but common version of the second-order Bernoulli Boolean model, false
negatives occur with probability 0 and false positives occur with probability
$0 < \varepsilon < 1$.

## Prediction

$B_T(x)$ is *correlated* with the latent value $x$, and thus provides information about the latent value. For instance, it allows one to predict $x$ given $B_T(x)$ better than if no observations where given whatsoever, assuming we know the error rate $\varepsilon(x)$ is better than
a random guess, e.g., in the case of $B_{\mathrm{bool}}$, $\varepsilon(x) < 0.5$.
If we have no prior information about the latent variable, the best (maximum likelihood) estimate of of its value is the observation $B_T(x)$.

However, because $\varepsilon(x)$ is normally known a priori, we can estimate the probability that the latent variable is $X = x$ using Bayes' rule:
$$
\Pr\\{X = x \mid B_T(x) = x \\} \propto \Pr\\{B_T(x) = x \mid X = x\\} \Pr\\{X = x\\},
$$
where $\Pr\\{X = x\\}$ is the prior probability that $X = x$ and $\Pr\\{B_T(x) = x \mid X = x\\}$ is the probability that $B_T(x) = x$ given that $X = x$ (the probability that the observation is correct). In the Bernoulli Boolean Model, for $X = 1$ this is the true positive rate $\tau$
and for $X = 0$ this is the true negative rate $\nu$.

In the first-order model, if the probability of being correct $\tau$, then:
$$
\Pr\\{x \mid B_{\mathrm{bool}}^{(1)}(x)\\} = \frac{\tau \Pr\\{X = x\\}}
    {\tau \Pr\\{X = x\\} + (1-\tau) (1-\Pr\\{X = x\\})}.
$$

Dividing the numerator and denominator by $\Pr\\{X = x\\}$, we get:
$$
\Pr\\{x \mid B_{\mathrm{bool}}^{(1)}(x)\\} = \frac{\tau}{\tau + (1-\tau) (1/\Pr\\{X = x\\} - 1)}.
$$

Assuming maximum ignorance (maximum entropy) about $x$ (i.e.,
$\Pr\\{X = 1\\} = 0.5$), the following expression is obtained for the first-order
Bernoulli Boolean Model, the above equation simplifies to $\Pr\\{x \mid B_{\mathrm{bool}}^{(1)}(x)\\} = \tau$.

Suppose we have $n$ noisy i.i.d. measurements of the latent variable $x$,
$$
\Bigl\\{ B_{1,\mathrm{bool}}^{(1)}(x), \ldots, B_{n,\mathrm{bool}}^{(1)}(x) \Bigr\\},
$$
in which case the maximum likelihood estimate of $x$ is the majority vote, i.e., the value
that occurs most frequently in the observations. As the number of independent sources
of observations goes to infinity, the majority vote converges in probability to $x$.

This is not a typical use-case for the Bernoulli Boolean model, since it will
mostly be a analytical result of probabilistic data structures that may be framed
in the context of a Bernoulli Model.

## Inducing Bernoulli Types

Here, we discuss how to generalize the results.

### Unit Functions

We discussed the idea of the Bernoulli Model for **value types** like $\mathrm{bool}$.
We can think of these Bernoulli Models as Bernoulli approximations of the set of unit types $() \mapsto X$. A unit type has no input, and it maps to a single constant value in the output. There are $|X|$ functions of this type.

If we replace $() \mapsto X$ with $B_X$ or, equivalently, $B_{() \mapsto X}$, we allow for the possibility of errors. The confusion matrix for $B_{() \mapsto \mathrm{bool}}^{(2)}$ is provided in Table 1.

Table 1: Second-order Bernoulli Boolean Model for $() \mapsto \mathrm{bool}$

| $x / B_{\mathrm{bool}}^{(2)}$ | observe $1$     | observe $0$           |
|-------------------------------|-----------------|-----------------------|
| **latent** $1$                | $\tau = 1-\eta$ | $\eta$                |
| **latent** $0$                | $\varepsilon$   | $\nu = 1-\varepsilon$ |

So, we might observe $B_{() \mapsto \mathrm{bool}}(x) = 1$ and, according to the confusion
matrix, the latent value $x$ is $1$ with probability $\tau$ (true positive rate) and $0$ with probability $\varepsilon$ (false positive rate). Likewise, we might observe $B_{() \mapsto \mathrm{bool}}(x) = 0$ and the latent value $x$ is $1$ with probability $\eta$ (false negative rate) and $0$ with probability $\nu$ (true negative rate).

We see that the Bernoulli Model for $() \mapsto \mathrm{bool}$ has a maximum order of 2, since $\eta$ and $\varepsilon$ are independent probabilities that fully describe the model. No additional free parameters are possible, since each row must sum to 1 (the total probability theorem), i.e., given a latent value $x$, the probability of observing $1$ or $0$ must sum to $1$ since that is the only possible two outcomes in the Bernoulli Boolean Model.

We can think of this as the asymmetric binary channel, where the probability of an error is different for $1$ and $0$.

A first-order Bernoulli Boolean model is a model where $\epsilon = \varepsilon = \eta$. See the confusion matrix in Table 2.

Table 2: First-Order Bernoulli Boolean Model for $() \mapsto \mathrm{bool}$

| $x / B_{\mathrm{bool}}^{(1)}$ | observe $1$              | observe $0$        |
|-------------------------------|--------------------------|--------------------|
| **latent** $1$                | $\tau =1-\epsilon$       | $\eta = \epsilon$  |
| **latent** $0$                | $\varepsilon = \epsilon$ | $\nu = 1-\epsilon$ |

We can see that there is only one free parameter, $\epsilon$, corresponding to the first-order Bernoulli Boolean Model. We can think of this as a binary symmetric channel, where the probability of an error is the same for $1$ and $0$.

For completeness, we can write down the confusion matrix for the zeroth-order model, which is just the standard Boolean model:

Table 3: Zeroth-Order Bernoulli Boolean Model for $() \mapsto \mathrm{bool}$

| $x / B_{\mathrm{bool}}^{(1)}$ | observe $1$ | observe $0$ |
|-------------------------------|-------------|-------------|
| **latent** $1$                | 1           | 0           |
| **latent** $0$                | 0           | 1           |

We see that there are 0 free parameters, and the model is deterministic.

#### Prediction: Boolean Values

Earlier, we discussed how to predict latent values from Bernoulli Model observations. Let's apply these insights to the first-order Bernoulli Model for $() \mapsto \mathrm{bool}$, which we can denote as $B_{\mathrm{bool}}^{(1)}$.

We can use Bayes' rule to compute the probability that the latent value is $X=1$ given that we observed $B_{\rm{bool}}^{(1)} = 1$:
$$
\Pr\\{X = 1 | B_{\rm{bool}}^{(1)} = 1\\} =
    \frac{
        \Pr\\{B_{\rm{bool}}^{(1)} =1 | X=1\\} \Pr\\{X = 1\\}
    }
    {
        \Pr\\{B_{\rm{bool}}^{(1)} = 1\\}
    }
$$
We know $\Pr\\{B_{\rm{bool}}^{(1)}=1 | X = 1\\}$ from the confusion matrix; it's just the true positive rate, $\tau = 1 - \epsilon$:
$$
\Pr\\{X=1 | B_{\rm{bool}}^{(1)}=1\\} = \frac
    {(1-\epsilon) \Pr\\{X = 1\\}}
    {\Pr\\{B_{\rm{bool}}^{(1)}=1\\}}
$$
What is $\Pr\\{B_{\rm{bool}}^{(1)} = 1\\}$? By the total probability theorem:
$$
\begin{split}
\Pr\\{B_{\rm{bool}}^{(1)} = 1\\} = \Pr\\{
    B_{\rm{bool}}^{(1)}=1 | X=1\\} \Pr\\{X = 1\\} + \\\
    \Pr\\{B_{\rm{bool}}^{(1)}=1 | X=0\\} \Pr\\{X = 0\\}.
\end{split}
$$
We can substitute in the confusion matrix values:
$$
\Pr\\{B_{\rm{bool}}^{(1)} = 1\\} = (1-\epsilon) \Pr\\{X = 1\\} + \epsilon \Pr\\{X = 0\\}.
$$

If we substitute this back into the expression for $\Pr\\{X=1 | B_{\rm{bool}}^{(1)}=1\\}$, divide the numerator and denominator by $\Pr\\{X = 1\\}$, and then simplify, we get
$$
\Pr\\{X=1 | B_{\rm{bool}}^{(1)}=1\\} = \frac
    {1-\epsilon}
    {1-\epsilon (1 - q/(1-q))}
$$
where $q = \Pr\\{X = 0\\}$.

Let's evaluate $q = \Pr\\{X = 0\\}$ at some interesting points:

1. At $q = 0$, $\Pr\\{X=1 | B_{\rm{bool}}^{(1)}=1\\} = 1$. This makes sense. We know that the latent value $1$ occurs with probability $1-q=1$, therefore whatever no matter what we observe, the latent value is $1$.
2. As $q \to 1$, $\Pr\\{X=1 |B_{\rm{bool}}^{(1)}=1\\} \to 0$. This also makes sense; we know that the latent value $1$ occurs with probability $q=1$, so no matter what we observe, the latent value is $0$.
3. At $q = 0.5$, $\Pr\\{X=1 |B_{\rm{bool}}^{(1)}=1\\} = 1-\epsilon$. This is the maximum entropy case, where we have no prior information about the latent value and assume maximum ignorance.

## Unary Bernoulli Functions

In this section, we expand our focus to unary functions.

### Lifing Unary Functions

If we have a function $f : \rm{bool} \mapsto \rm{bool}$, then the space of all possible functions is given by Table 4.

Table 4: All possible functions of type $\rm{bool} \mapsto \rm{bool}$

| $f$          | $f(\rm{true})$ | $f(\rm{false})$ |
|--------------|----------------|-----------------|
| $\rm{id}$    | $\rm{true}$    | $\rm{false}$    |
| $\rm{not}$   | $\rm{false}$   | $\rm{true}$     |
| $\rm{true}$  | $\rm{true}$    | $\rm{true}$     |
| $\rm{false}$ | $\rm{false}$   | $\rm{false}$    |

Suppose we replace the Boolean inputs with Bernoulli Boolean values and ask the question, "What is the probability that $f\bigl(B_{\rm{bool}}^{(1)}(x)\bigr) = f(x)$?"

Notice that $f\bigl(B_{\rm{bool}}^{(1)}(x)\bigr)$ is $f(x)$ with some probability, but $f(x)$ may be latent depending on $f$. For the constant fuctions, $\mathrm{true}$ and $\mathrm{false}$, we get the same function, i.e., $\mathrm{true} : B_{\rm{bool}}^{(k)} \mapsto B_{\rm{bool}}^{(0)} \equiv \mathrm{true} : \mathrm{bool} \mapsto \mathrm{bool}$ since $\mathrm{true} : \mathrm{bool} \mapsto \mathrm{bool}$ always outputs $\mathrm{true}$, and similiarly for $\mathrm{false} : \mathrm{bool} \mapsto \mathrm{bool}$.

However, the $\mathrm{id}$ and $\mathrm{not}$ functions are different. For instance, suppose
$\\Pr\\{B_{\rm{bool}}^{(1)}(x) = x\\} = p$. Then, when we input
$B_{\rm{bool}}^{(1)}(\mathrm{true})$ into $\mathrm{id}$, we get the correct output $\mathrm{true}$ with probability $p$ and the incorrect output $\mathrm{false}$ with probability $1-p$. Likewise,when we input $B_{\mathrm{bool}}^{(1)}(\mathrm{false})$ into $\mathrm{id}$, we get the correct output $\mathrm{false}$ with probability $p$ and the incorrect output $\mathrm{true}$ with probability $1-p$.

Since we can think of these outputs as either correct or incorrect with probability $p$ and $1-p$ respectively, they are Bernoulli Boolean values, e.g., this $\mathrm{id}$ function on Bernoulli Booleans is of type $B_{\mathrm{bool}}^{(1)} \mapsto B_{\mathrm{bool}}^{(1)}$. We monadically lift $\mathrm{id} : \mathrm{bool} \mapsto \mathrm{bool}$ to a function of type $B_{\mathrm{bool}}^{(1)} \mapsto B_{\mathrm{bool}}^{(1)}$.

Notice that this is not a Bernoulli Model of $B_{\mathrm{bool} \mapsto \mathrm{bool}}$, but rather a function of type $B_{\mathrm{bool}} \mapsto B_{\mathrm{bool}}$. This may surprise the reader, but it is important to think about how these Bernoulli Models compose.

For instance, if we have the function $\mathrm{true} : \mathrm{bool} \mapsto \mathrm{bool}$, then when we provide it with a Bernoulli Boolean value, we know that we $\mathrm{true}$ is the correct output -- there are no latent values. However, a Bernoulli Model of the (function) $\mathrm{true} : \mathrm{bool} \mapsto \mathrm{bool}$ is a completely different concept. The Bernoulli approximation of this funcction is of type $B_{\mathrm{bool} \mapsto \mathrm{bool}}$, and the latent function, $\mathrm{true} : \mathrm{bool} \mapsto \mathrm{bool}$, is not observable, but $B_{\mathrm{bool} \mapsto \mathrm{bool}}(\mathrm{true})$ is observable and provides some information about the latent function.

Presumably, some process generated the Bernoulli approximation $B_{\mathrm{bool} \mapsto \mathrm{bool}}(\mathrm{true})$, and we wish to use that approxmiate function as a replacement for the latent function we are actually interested in, which is $\mathrm{true} : \mathrm{bool} \mapsto \mathrm{bool}$.

There are only 4 possible functions of type $\mathrm{bool} \mapsto \mathrm{bool}$, and in Table 5 we show the confusion matrix for the highest-order model, $B_{\mathrm{bool} \mapsto \mathrm{bool}}^{(12)}$.

Table 5: Bernoulli Model for $\mathrm{bool} \mapsto \mathrm{bool}$

|                       | observe $\mathrm{id}$ | observe $\mathrm{not}$ | observe $\mathrm{true}$ | observe $\mathrm{false}$ |
|-------------------------|--------------|---------------|----------------|-----------------|
| latent $\mathrm{id}$    | $p_{1 1}$    | $p_{1 2}$     | $p_{1 3}$      | $p_{1 4}$       |
| latent $\mathrm{not}$   | $p_{2 1}$    | $p_{2 2}$     | $p_{2 3}$      | $p_{2 4}$       |
| latent $\mathrm{true}$  | $p_{3 1}$    | $p_{3 2}$     | $p_{3 3}$      | $p_{3 4}$       |
| latent $\mathrm{false}$ | $p_{4 1}$    | $p_{4 2}$     | $p_{4 3}$      | $p_{4 4}$       |

Each row must sum to 1, $\sum_j p_{i j} = 1$, so we only have up to a maximum of
$4 (4-1) = 12$ degrees of freedom. This means the highest Bernoulli Boolean order is
12, but we normally drop the order information and just write $B_{\mathrm{bool} \mapsto \mathrm{bool}}$ (and propogate error rates using interval arithmetic).

Often, the order is either first-order or for various reasons. The first-order
Bernoulli Model of $\mathrm{bool} \mapsto \mathrm{bool}$ is a model where every entry in the confusion matrix is a function of some single value. The maximum entropy
confusion matrix, given error rates $\epsilon$, is given in Table 6.

Table 5: Bernoulli Model for $\mathrm{bool} \mapsto \mathrm{bool}$

|| observe $\mathrm{id}$ | observe $\mathrm{not}$ | observe $\mathrm{true}$ | observe $\mathrm{false}$ |
|-----------------------------|--------------|--------------|--------------|--------------|
| **latent** $\mathrm{id}$    | $1-\epsilon$ | $\epsilon/3$ | $\epsilon/3$ | $\epsilon/3$ |
| **latent** $\mathrm{not}$   | $\epsilon/3$ | $1-\epsilon$ | $\epsilon/3$ | $\epsilon/3$ |
| **latent** $\mathrm{true}$  | $\epsilon/3$ | $\epsilon/3$ | $1-\epsilon$ | $\epsilon/3$ |
| **latent** $\mathrm{false}$ | $\epsilon/3$ | $\epsilon/3$ | $\epsilon/3$ | $1-\epsilon$ |

When we have a Bernoulli Model approximation of some latent function of type
$\mathrm{bool} \mapsto \mathrm{bool}$, we wish to store the error information in the output so that we can propagate error information through the computation.

We do this by saying that the output is a Bernoulli Boolean, because it may or may not be correct, i.e., the Bernoulli Model generates a function of type $\mathrm{bool} \mapsto B_{\mathrm{bool}}$ where the output is a Bernoulli Boolean value. In our algorithms, we created a type system for this, using interval arithmetic to propagate the error rates through the computation.

Notice that the Bernoulli Model on $\mathrm{bool} \mapsto \mathrm{bool}$ does not change the type of the input, $\mathrm{bool}$. We can, of course, also provide as input to this function a Bernoulli Booleans, in which case we will usually get an even higher-order Bernoulli Boolean as output.

Since functions are values, we can ask the question, what is the probability
that the latent function is equal to the observed function:
$$
\Pr\\{ B_{\mathrm{bool} \mapsto \mathrm{bool}}(f) = f\\}.
$$

In this case, we are asking about the equality of the functions, which is mathematically equivalent to asking whether each input in the domain maps to the same output as the latent function:
$$
\begin{split}
\Pr\\{B_{\mathrm{bool}\mapsto \mathrm{bool}}^{(1)}(\mathrm{id})(\mathrm{true}) = \mathrm{id}(\mathrm{true}) \\} \times \\\ \Pr\\{B_{\mathrm{bool}\mapsto \mathrm{bool}}^{(1)}(\mathrm{id})(\mathrm{false}) = \mathrm{id}(\mathrm{false}) \\}
\end{split}
$$

From the confusion matrix, we know this product of probabilities is $1-\epsilon$. In fact, normally, the process that generates the Bernoulli Model of the latent function is based on these kinds of probabilities on individual inputs.

### Higher-Order Bernoulli Models

If we are trying to estimate the latent function, a higher order complicates the estimation problem (more parameters to estimate). However, a higher-order may be *desirable* in many cases, since it allows for more capacity to approximate the latent function. In general, when looking at the confusion matrix, we want the diagonal to be as close to 1 as possible. For the off-diagonal elements, we want functions that are more similiar to the latent function to have larger probabilities than functions that are less similiar to the latent function. This is just a way of
minimizing a loss function.

## Set-Indicator Functions

The set-indicator function is a function that maps an element of a set to a
Boolean value. We can think of this as a function of type $X \mapsto \mathrm{bool}$,
which returns true if the input is in the set and false otherwise.

Let us consider Bernoulli Models for set-indicator functions. The Bloom filter,
for instance, is a Bernoulli model of the set-indicator function. It is a second-order
model, since false negatives occur with probability 0 and false positives occur with
probability $\varepsilon$. This is actually the *expectation* of the false positive
rate, and the true false positive rate is a random variable that cannot usually be
computed unless $X$ is a finite set.

### HashSet: Approximate Set-Indicator Functions

Suppose we have a cryptographic hash function $h : X \mapsto \{0,1\}^n$, where
$n$ is the number of bits in the hash value. We define an approximate set-indicator
function, denoted by $\mathrm{HashSet}$, in the following way:

- We are given a set $A$ to (approximately) represent.
- We find a seed $s$ such that $h(x s) = 0^n$ for all $x \in A$.
- We do not look at any $x \notin A$, and by the properties of $h$, for $x \notin A$, $h(x s) = 0^n$ with probability $2^{-n}$ corresponding to a false positive, and otherwise $h(x s) \neq 0^n$ with probability $1-2^{-n}$.
- We define membership as $x \in A \equiv h(x s) = 0^n$.

So, we construct a $\mathrm{HashSet}$ by finding a seed $s$ such that $h(x s) = 0^n$ and that is
the only value we need to store, but since the probability that all $x \in A$ hash to $0^n$ occurs
with probability $2^{-n}$, each trial is Bernoulli distributed with probability
$$
    \prod_{j=1}^{|A|} 2^{-n} = 2^{-n|A|},
$$
which will take an expected number of trials $2^{n|A|}$. We can store the seed as a string of $n|A|$ bits, or $n$ bits per element in the set $A$. Since it has a false positive rate $\varepsilon = 2^{-n}$, $n = -\log_2 \varepsilon$, and we can reparametrize the space complexity as $-\log_2 \varepsilon$ bits per element. This is the information theoretic lower-bound, but we achieved it only by using an algorithm with exponential time complexity. We can do better by using different algorithms, but it comes at a cost to space complexity.


### Bernoulli Model for Set-Indicator Functions

The number of functions of type $X \mapsto \mathrm{bool}$ is $2^{|X|}$. We can
think of these as the set of all possible set-indicator functions. We can
approximate this set of functions with a Bernoulli Model, which we denote
$B_{X \mapsto \mathrm{bool}}$.

We can use the simple $\mathrm{HashSet}$ construction to induce the Bernoulli Model for set-indicator functions. Specifically, in the $\mathrm{HashSet}$, false positives occur with some positive probability $\epsilon$ and false negatives occur with probability $0$.

Technically, however, the Bernoulli Model is being applied to the set-indicator function, but this also specifies a Bernoulli Model on the Boolean output of the set-indicator function:

$$
    \in : X \times \mathcal{P}(X) \mapsto B_{\mathrm{bool}}.
$$

Let's consider $X = \\{1,2\\}$ and $A = \\{2\\}$. The confusion matrix for the this contruction is given in Table 7.

Table 7: Bernoulli Model for $X \mapsto \mathrm{bool}$

|               | $1_\emptyset$ | $1_{\\{1\\}}$ | $1_{\\{2\\}}$ | $1_{\\{1,2\\}}$ |
|---------------|---------------|-------------|-------------|---------------|
| $1_\emptyset$ | $(1-\epsilon)^2$ | $\epsilon(1-\epsilon)$ | $\epsilon(1-\epsilon)$ | $\epsilon^2$ |
| $1_{\\{1\\}}$   | $0$           | $1-\epsilon$ | $0$ | $\epsilon$ |
| $1_{\\{2\\}}$   | $0$           | $0$         | $1-\epsilon$ | $\epsilon$ |  
| $1_{\\{1,2\\}}$ | $0$           | $0$         | $0$         | $1$           |

We do not know the latent set $A = \\{2\\}$, we only have the approximation
$B_{X \mapsto \mathrm{bool}}(A)$, and we use this as a replacement for the latent set $A$.

Notice that if we start with $A = \\{2\\}$, in row 3, we have zeros in the
first two columns for the empty set and $\\{1\\}$, which makes sense
as by construction no false negatives are possible, only false positives.

We see that with probability $1-\epsilon$, the output is correct, and with
probability $\epsilon$, the output is incorrect. This is a Bernoulli Model of
the set-indicator function (or Bernoulli Set), and we can use this to provide information
about the latent set.

On the equality of Bernoulli Sets, we can ask the question, what is the probability
that $B_{X \mapsto \mathrm{bool}}(A) = A$? This is just $1-\epsilon$, as shown
in the confusion matrix. However, this is not typically what people are interested
in. They are interested in the probability that an element that tests positive
is in fact in the latent set $A$:
$$
\Pr\\{x \in B_{X \mapsto \mathrm{bool}}(A)\\}.
$$

This is just the true positive rate, which is $1-\epsilon$ by construction. Thus, false positives occur with probability $\epsilon$, and false negatives occur with probability 0.
We see that when we do membership tests, we can think of the output as a Bernoulli
Boolean value:
$$
    x \in B_{X \mapsto \mathrm{bool}}(A) : X \mapsto \mathrm{bool}^{(2)}.
$$

## Conclusion

The Bernoulli Model is a way of thinking about the uncertainty in the output of a function, and how that uncertainty propagates through a computation, and typically the uncertainty is due to a trade-off between space complexity and accuracy. The more space we use to represent the function, the more closely it is expected to approximate the latent function.
