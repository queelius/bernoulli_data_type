

TODO: in proofs, start referring to axioms and theorems and postulates instead of "by independence" or other similar things.




TODO: incorporate sigma stuff into it, (F, P, P(U)) or something or other.

TODO: derive result for finite universe. what's optimal (LB) space complexity given that false positive rate is 0 and false negative rate is 0? prelim math says u bits, or u/m bits per key. iterate over positives, find a singular hash for them, and also iterate over all negatives and just make sure that none of them hash to singular hash value previously given. if hash size is N=2, then u bits. can we do better?

negatives: each one is (N-1)/N. positives: each one is 1/N

p = (1/N)^m * ((N-1)/N)^(u-m)

argmin{N} log(1/[(1/N)^m * ((N-1)/N)^(u-m)])




TODO: binomial distribution is maximum entropy distribution given constraint that $\expectation[\FPR_\n] = \varepsilon$. Entropy is ?. 



\begin{theorem}
The \emph{intersection} of approximate sets $\Sa_1$ and $\Sa_2$ respectively with false positive rates $\varepsilon_1$ and $\varepsilon_2$ and false negative rates $\eta_1$ and $\eta_2$ is an \emph{approximate set} with a false negative rate
\begin{equation}
    \eta = \eta_1 + \eta_2 - \eta_1 \eta_2\,.
\end{equation}
and a false negative rate
\begin{equation}
    \varepsilon = \alpha_1 \varepsilon_1 (1 - \eta_2) + \alpha_2 \varepsilon_2 (1 - \eta_1) + (1 - \alpha_1 - \alpha_2) \varepsilon_1 \varepsilon_2
\end{equation}
where
\begin{equation}
\label{eq:union_fn_rate_alpha}
   \alpha_1 = \frac{\card{\St_1 \! \setminus \St_2}}{\card{\St_1 \! \cup \St_2}}\;\text{and}\;\alpha_2 = \frac{\card{\St_2 \! \setminus \St_1}}{\card{\St_1 \! \cup \St_2}}\,.
\end{equation}
\end{theorem}
\begin{proof}
We already know how to compute the false negative and false positive rates that result from complements and unions. So, if we transform an intersection into a sequence of complements and unions, we can compute the false negative and false positive rates of the intersection. Observe that
\begin{align}
    \eta &= \Pr[\rv{X} \in \setc{\Sa_1 \cap \Sa_2} \given \rv{X} \in \St_1 \cap \St_2]\\
         &= \Pr[\rv{X} \in \setc{\Sa_1} \cup \setc{\Sa_2} \given \rv{X} \in \St_1 \cap \St_2]\,.
\end{align}
\end{proof}



\begin{corollary}
The intersection of positive approximate sets $\Sp_1$ and $\Sp_2$ respectively with false positive rates $\varepsilon_1$ and $\varepsilon_2$ is a positive approximate set with a false positive rate
\begin{equation}
    \varepsilon = \alpha_1 \varepsilon_1 + \alpha_2 \varepsilon_2 + (1 - \alpha_1 - \alpha_2) \varepsilon_1 \varepsilon_2\,,
\end{equation}
where
\begin{equation}
\alpha_1 = \frac{\card{\St_1 \! \setminus \St_2}}{\card{\St_1 \! \cup \St_2}} \; \text{and} \; \alpha_2 = \frac{\card{\St_2 \! \setminus \St_1}}{\card{\St_1 \! \cup \St_2}}\,.
\end{equation}
\end{corollary}

\begin{corollary}
The intersection of negative approximate sets $\Sn_1$ and $\Sn_2$ respectively with false negative rates $\eta_1$ and $\eta_2$ is a negative approximate set with a false negative rate
\begin{equation}
    \eta = \eta_1 + \eta_2 - \eta_1 \eta_2\,.
\end{equation}
\end{corollary}

\begin{corollary}
The union of positive approximate sets $\Sp_1$ and $\Sp_2$ respectively with false positive rates $\varepsilon_1$ and $\varepsilon_2$ is a positive approximate set with a false positive rate
\begin{equation}
    \varepsilon = \varepsilon_1 + \varepsilon_2 - \varepsilon_1 \varepsilon_2\,.
\end{equation}
\end{corollary}

\begin{corollary}
The union of negative approximate sets $\Sn_1$ and $\Sn_2$ respectively with false negative rates $\eta_1$ and $\eta_2$ is a negative approximate set with a false negative rate
\begin{equation}
    \eta = \alpha_1 \eta_1 + \alpha_2 \eta_2 + (1 - \alpha_1 - \alpha_2) \eta_1 \eta_2\,,
\end{equation}
where
\begin{equation}
\alpha_1 = \frac{\card{\St_1 \! \setminus \St_2}}{\card{\St_1 \! \cup \St_2}} \; \text{and} \; \alpha_2 = \frac{\card{\St_2 \! \setminus \St_1}}{\card{\St_1 \! \cup \St_2}}\,.
\end{equation}
\end{corollary}

\begin{theorem}
The \emph{negative approximate set} is closed under union and intersection and the \emph{complement} of a negative approximate set with a false negative rate $\eta$ is an \emph{approximate set} with a false positive rate $\varepsilon = \eta$.
\end{theorem}
The union or intersection of an approximate set and a negative approximate set produces a \emph{dirty} set that may \emph{false negatives} and \emph{false positives}.



Compute fp rate/fn rate for union A + B by (A'B')'. same?

compute the optimal efficiency under set-theoretic compositions of approximate sets when elements aren't iterable, i.e., the approximate set data structures must be composed together.

then, show the efficiency of the bloom filter and perfect hash filter.
-------




============
Show how boolean_search(q, D*) generates an approximate finite set of boolean_search(q, D), where D* consists of approximate sets of elements in D.

Once this is done, all other proven properties of approximate sets immediately follows, like in linear algebra... prove something is a vector space and we immediately get all kinds of results.



calculate the false positive rate or false negative rate in set theory stuff.



think about varepsilon for unions / intersections when varepsilon is always the same. simplified solution. show it for in general n unions...etc

instead of no false negatives, say true positive rate is 1 (false negative rate is 0).

define true positive rate. then, use this def. in the proof that result set R* is approx set.















\begin{proof}
Consider two approximate sets, $\Sp_1$ and $\Sp_2$ with false positive rates $\varepsilon_1$ and $\varepsilon_2$ over the infinite universe $\mathbb{U}$.

The probability of a false positive for the approximate set $\Sp_1 \! \cup \Sp_2$ is given by
\begin{equation}
\label{eq:var_proof_union_inf}
    \varepsilon = \Pr\left[\rv{X} \in \Sp_1 \! \cup \Sp_2 \given \rv{X} \notin \St_1 \! \cup \St_2 \right]\,,
\end{equation}
where $\rv{X}$ is a randomly chosen element in $\mathbb{U}$. By \Cref{}, $\Sp_1$ and $\Sp_2$ are infinite and since $\St_1$ and $\St_2$ are finite, the probability that $\rv{X}$ realizes a value not in $\St_1 \cup \St_2$ is unity. Thus, \Cref{eq:var_proof_union_inf} may be rewritten as
\begin{align}
    \varepsilon
        &= \Pr\left[\rv{X} \in \Sp_1 \! \cup \Sp_2\right]\\
        &= 1 - \Pr\left[\rv{X} \notin \Sp_1 \!  \cap \rv{X} \notin \Sp_2\right]\,.
\end{align}
The events $\rv{X} \in \Sp_1$ and $\rv{X} \in \Sp_2$ are independent and thus
\begin{equation}
    \varepsilon = 1 - \Pr\left[\rv{X} \notin \Sp_1\right]\Pr\left[\rv{X} \notin \Sp_2\right]\,.
\end{equation}
The probability that $\rv{X}$ realizes a value not in $\Sp_j$ is given by $1 - \varepsilon_j$, and thus
\begin{equation}
    \varepsilon = 1 - (1 - \varepsilon_1) (1 - \varepsilon_2)\,.
\end{equation}
\end{proof}




















\section{Proof of Theorem~\ref{thm:intersect_fp_rate}}
According to \cref{thm:intersect_fp_rate}, given a universe of $u$ elements and approximate sets $\Sa_1$ and $\Sa_2$ respectively with false positive rates $\varepsilon_1$ and $\varepsilon_2$ and false negative rates $\eta_1$ and $\eta_2$, the intersection $\Sa_1 \! \cap \Sa_2$ is an approximate set with a false positive rate given by
\begin{equation*}
\begin{split}
    \varepsilon &= \alpha_1 \varepsilon_1 \left(1 - \eta_2\right) + \alpha_2 \varepsilon_2 \left(1 - \eta_1\right) + \left(1 - \alpha_1 - \alpha_2\right) \varepsilon_1 \varepsilon_2
\end{split}
\tag{\ref{eq:intersect_fn_rate} revisited}
\end{equation*}
where
\begin{equation*}
    0 \leq \alpha_1 = \frac{\card{\St_1 \setminus \St_2}}{u - \card{\St_1 \! \cap \St_2}}\,,
    0 \leq \alpha_2 = \frac{\card{\St_2 \setminus \St_1}}{u - \card{\St_1 \! \cap \St_2}}\,,
    \textrm{and} \; \alpha_1 + \alpha_2 \leq 1\,.
\end{equation*}
\begin{proof}
The false positive rate is equivalent to the ratio of the \emph{expected} number of false positives to the total number of possible false positives. The set of \emph{possible} false positives are given by the are given by the disjoint sets
\begin{equation}
    \St_1 \setminus \St_2\,, \St_2 \setminus \St_1\,, \text{and} \; \mathbb{U} \setminus \St_1 \cup \St_2\,.
\end{equation}
Since they are disjoint, we may consider each independently to calculate the expected number false positives.

Suppose we randomly select an element from the universe $\mathbb{U}$, denoted by $\rv{X}$. Given that $\rv{X} \in \mathbb{U} \setminus \St_1 \cup \St_2$, $\rv{X}$ is a negative in both $\St_1$ and $\St_2$, the probability that $\rv{X}$ is positive in both of the approximate sets $\Sa_1$ and $\Sa_2$ is given by
\begin{equation}
    \Pr[\rv{X} \in \Sa_1 \! \cap \rv{X} \in \Sa_2 \given \rv{X} \notin \St_1 \cap \rv{X} \notin \St_2]\,.
\end{equation}
Since these are independent events, the above probability is equivalent to
\begin{equation}
    \Pr[\rv{X} \in \Sa_1 \given \rv{X} \notin \St_1] \Pr[\rv{X} \in \Sa_2 \given \rv{X} \notin \St_2]\,.
\end{equation}
By definition, $\Pr[\rv{X} \in \Sa_j \given \rv{X} \notin \St_j]$ is the false positive rate $\varepsilon_j$ and thus the above probability is given by
\begin{equation}
    \varepsilon_1 \varepsilon_2\,.
\end{equation}
By the rules of set theory, there are $u - (m_1 + m_2 - m_{1 2})$ elements in $\mathbb{U} \setminus \St_1 \cup \St_2$, where each one is independent Bernoulli trial. Thus, there are expected to be
\begin{equation}
    (u - (m_1 + m_2 - m_{1 2})) \varepsilon_1 \varepsilon_2
\end{equation}
false positives in $\mathbb{U} \setminus \St_1 \cup \St_2$.

The elements in $\St_2 \setminus \St_1$ are negatives in $\St_1$ and positives in $\St_2$. Therefore, the probability that $\rv{X}$ is a positive in both of the approximate sets $\Sa_1$ and $\Sa_2$ is given by
\begin{equation}
    \Pr[\rv{X} \in \Sa_1 \! \cap \rv{X} \in \Sa_2 \given \rv{X} \notin \St_1 \! \cap \rv{X} \in \St_2]\,.
\end{equation}
Since these are independent events, the above probability is equivalent to
\begin{equation}
    \Pr[\rv{X} \in \Sa_1 \given \rv{X} \notin \St_1] \Pr[\rv{X} \in \Sa_2 \given \rv{X} \in \St_2]\,.
\end{equation}
By definition, $\Pr[\rv{X} \in \Sa_1 \given \rv{X} \notin \St_1]$ is the false positive rate $\varepsilon_1$ and $\Pr[\rv{X} \in \Sa_2 \given \rv{X} \in \St_2]$ is the true positive rate $1 - \eta_2$ and thus the above probability is given by
\begin{equation}
    \varepsilon_1 (1 - \eta_2)\,.
\end{equation}
By the rules of set theory, there are $m_2 - m_{1 2}$ elements in $\St_2 \setminus \St_1$, where each one is an independent Bernoulli trial. Thus, there are expected to be
\begin{equation}
    (m_2 - m_{1 2}) \varepsilon_1 (1 - \eta_2)
\end{equation}
false positives in $\St_2 \setminus \St_1$. A similar argument follows for $\St_1 \setminus \St_2$ where there are expected to be
\begin{equation}
    (m_1 - m_{1 2}) \varepsilon_2 (1 - \eta_1) 
\end{equation}
false positives.

Since we have computed the expected number of false positives over every possible false positive, the false positive rate $\varepsilon$ is given by
\begin{equation}
\begin{split}
    \varepsilon =
        & \left(\frac{m_2 - m_{1 2}}{u - m_{1 2}}\right) \varepsilon_1 \left(1 - \eta_2\right) + \left(\frac{m_1 - m_{1 2}}{u - m_{1 2}}\right) \varepsilon_2 \left(1 - \eta_1\right)\\
        &\qquad+ \left(\frac{u - m_1 - m_2 + m_{1 2}}{u - m_{1 2}}\right) \varepsilon_1 \varepsilon_2\,.
\end{split}
\end{equation}
\end{proof}














\begin{theorem}
\label{thm:approx_expected_precision}
Given a universe of $u$ elements where $\alpha \cdot 100\%$ are positive, an approximate set with a false positive rate $\varepsilon$ and a false negative rate $\eta$ has an \emph{expected} precision given \emph{approximately} by
\begin{equation}
\label{eq:cor_approx_expected_precision}
    \gamma(\alpha, u, \eta, \varepsilon) \approx \frac{\overline{t}}{\overline{t} + \overline{f}} +
    \frac{\overline{t} \sigma_{\!f}^2 - \overline{f} \sigma_{\!t}^2}{\left(\overline{t} + \overline{f}\right)^3}\,,
\end{equation}
where $\overline{t} = u \alpha(1 - \eta)$ is the \emph{expected} number of \emph{true positives}, $\overline{f} =  u(1-\alpha)\varepsilon$ is the \emph{expected} number of \emph{false positives}, $\sigma_{\!t}^2 = u \alpha(1 - \eta) \eta$ is the variance of the number of \emph{true positives}, and $\sigma_{\!f}^2 = u(1-\alpha)\varepsilon(1-\varepsilon)$ is the variance of the number of \emph{false positives}.
\end{theorem}












\subsection{Restricted set-theoretic model}
Positive (negative) approximate sets are not closed under the set-theoretic model due to the reliance upon \emph{complements}. The \emph{restricted} set-theoretic model that is \emph{closed} for positive or negative approximate sets is given by \cref{bnf:restricted_set_theoretic}. If intersections (or unions) are taken with respect to two \emph{positive} approximate sets, or two \emph{negative} approximate sets, the operations are closed. Moreover, the complement of an approximate set is a negative approximate set and vice versa. Thus, the restricted set-theoretic model is guaranteed to produce either a \emph{positive} approximate set or a \emph{negative} approximate set.

\begin{floatbnf}
\caption{Restricted set-theoretic model for positive or negative approximate sets}
\label{bnf:restricted_set_theoretic}
\begin{bnf*}
    \bnfprod{exp}
        \bnfpn{aset} \bnfor \bnfts{\Union\!(} \bnfpn{exp}\bnfts{,}\bnfpn{exp}\bnfts{)} \bnfor \bnfts{\Complement\!(}\bnfpn{nexp}\bnfts{)}\\
    \bnfprod{nexp}
        \bnfpn{naset} \bnfor \bnfts{\Union\!(} \bnfpn{nexp}\bnfts{,}\bnfpn{nexp}\bnfts{)} \bnfor \bnfts{\Complement\!(}\bnfpn{exp}\bnfts{)}\\
    \bnfprod{aset} \bnftd{a \emph{positive} approximate set}\\
    \bnfprod{naset} \bnftd{a \emph{negative} approximate set}
\end{bnf*}
\end{floatbnf}














\begin{table}[h]
    \centering
    \begin{tabular}{|c c|}
        \multicolumn{2}{c}{Summary of performance measures}\\[5pt]
        \hline
        \textbf{measure} & \textbf{expected value}\\[5pt]
        \hline
         recall ($r$) & $1 - \eta$\\[5pt]
         precision ($\gamma$) & $\frac{\lambda(1-\eta)}{\lambda(1-\eta) + (1-\lambda)\varepsilon}+\frac{\lambda(1-\lambda)\varepsilon(1-\eta)(1-\varepsilon-\eta)}
        {u \left(\lambda(1-\varepsilon-\eta)+\varepsilon\right)^3}$\\[5pt]
        accuracy ($\rho$) & $1 - \left(1 - \lambda\right) \varepsilon - \lambda \eta$\\[5pt]
        negative predictive value ($\upsilon$)& ?\\[5pt]
        specificity ($s$) & $1 - \varepsilon$\\[5pt]
        miss rate & $\eta$\\[5pt]
        fall out & $\varepsilon$\\[5pt]
        false discovery rate & $1-\gamma$\\[5pt]
        false omission rate & $1-\upsilon$\\[5pt]
        \hline
    \end{tabular}
    \caption{Various \emph{expected} performance measures for an approximate set $\Sa$ with a false positive rate $\varepsilon$ and false negative rate $\eta$, where the set $\St$ has $\lambda u$ positives from a universe of $u$ elements.}
    \label{tab:perf_sum}
\end{table}





















\begin{table}[h]
    \centering
    \caption{Set-theoretic operations of approximate sets $\Sa_1$ and $\Sa_2$ respectively with false positive rates $\varepsilon_1$ and $\varepsilon_2$ and false negative rates $\eta_1$ and $\eta_2$. As a proportion of positives, $\alpha_1$ is the proportion exclusive to $\St_1$, $\alpha_2$ is the proportion exclusive to $\St_2$, and $\alpha_3$ is the proportion in both.}
    \label{tab:bin_op}    
    \begin{tabular}{@{} l l l @{}}
        \toprule
        \textbf{operation} & \textbf{false positive rate} $\varepsilon$ & \textbf{false negative rate} $\eta$ \\
        \midrule
         $\Sa_1 \! \cup \Sa_2$ & $1 - (1 - \varepsilon_1)(1- \varepsilon_2)$ & $\alpha_1 \eta_1(1 - \varepsilon_2) + \alpha_2 \eta_2(1 - \varepsilon_1) + \alpha_3 \eta_1 \eta_2$\\
         $\Sa_1 \! \cap \Sa_2$ & $\alpha_1 \varepsilon_1(1-\eta_2) + \alpha_2 \varepsilon_2(1 - \eta_1) + \alpha_3 \varepsilon_1 \varepsilon_2$ & $1-(1-\eta_1)(1-\eta_2)$\\
         $\Sa_1 \! \setminus \Sa_2$ & $\alpha_1 \varepsilon_1(1-\varepsilon_2) + \alpha_2 \eta_2(1 - \eta_1) + \alpha_3 \varepsilon_1 \eta_2$ & $\eta_1 + \varepsilon_2(1-\eta_1)$\\
         $\setc{\Sa_1}$ & $\eta_1$ & $\varepsilon_1$\\
         \bottomrule
    \end{tabular}
\end{table}















%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

The distribution of false positives and false negatives are Bernoulli distributed random variables conditioned on a particular number of positives. The distribution of positives (and negatives) is given by the following definition.
\begin{definition}
The number of \emph{positives} in a universe of $u$ elements is uncertain. We model the uncertainty as a discrete random variable, denoted by $\POS$, with a probability mass function\footnote{The probability mass function of a random variable $\RV{X}$ is denoted by $\PDF{\,\cdot\,}[\RV{X}]$.}
\begin{equation}
    \PDF{p \Given u}[\POS]
\end{equation}
and a support $\{0,\ldots,u\}$. Conversely, the distribution of negatives is a random variable $\NEG = u - \POS$.
\end{definition}
A parametric model of the number of random positives $\POS$ is unlikely; if \emph{data} is available, let the \emph{data} speak for itself and use an empirical distribution for $\POS$. The joint probability mass function of positives, false positives, and false negatives is given by
\begin{equation}
\begin{split}
    \PDF{p, f_p, f_n \Given u, \fprate, \fnrate}[\POS,\FP,\FN] =\\
    \PDF{p \Given u}[\POS] \PDF{f_p \Given p, u, \fprate}[\FP] \PDF{f_n \Given p, u, \fnrate}[\FN]\,.
\end{split}
\end{equation}
The joint distribution fully specifies the distribution of the classification measures in \cref{tbl:perf_sum}, i.e., it may be used to provide accurate \emph{point} estimates.
\begin{example}
The \emph{expected} precision that is not dependent upon a given proportion of positives $\lambda$ is given by
\begin{equation}
    \Expect{\ppv\!\left(\fnrate, \fprate; \Lambda, u\right)} =
    \sum_{p=0}^{u} \ppv\!\left(\fnrate,\fprate; \frac{p}{u},u\right) \PDF{\frac{p}{u} \Given u}[\Lambda]\,,
\end{equation}
where $\Lambda = \frac{\POS}{u}$.
\end{example}












%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Talk about dynamic and statis, pros and cons.

\section{Static polymorphism}
Talk about generics and concepts here. Show how there is no need for an interface type to be inherited from. You still can do that, of course, and you can also implement a concept checker to verify that a data structure is an approximate set type.

\subsection{Set-theoretic model}







%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <template <class> Interval, class Iterator>
using interval_t = Interval<typename std::iterator_traits<Iterator>::value_type>;

template <class Interval, class ForwardIterator>
auto interval_span(ForwardIterator begin, ForwardIterator end)
{
    using base_type = typename std::iterator_traits<ForwardIterator>::value_type;
    
    base_type min = std::numeric_limits<T>::max();
    base_type max = std::numeric_limits<T>::min();
    for (auto interval = begin; interval != end; ++interval)
    {
        min = std::min(min, begin->lower());
        max = std::max(max, begin->upper());
    }
    return Interval<base_type>(min, max);
};



















================

and let $\RV{X_j}$ denote the outcome of the $j$-th membership test, where $\RV{X_j} = 1$ is a false positive and $\RV{X_j} = 0$ is a true negative for $j=1,\ldots,\n$.
\begin{equation}
    \vec{X}_\n = \Tuple{\RV{X_1},\RV{X_2},\ldots,\RV{X_\n}} \in \{0,1\}^{\n}\,.
\end{equation}










**********************
he set-union operator overP(U)is acommutative monoidwith the identity element∅.The set of approximate sets with expected false positive and false negative ratesεandωrespectively is not even a semigroup under the set-union operator, since by??the union of twoapproximate sets is an approximate set with uncertain false positive and false negative rates, e.g.,their union has an expected false positive rate bounded by the interval1−(1−[ε])2.The set of approximate sets with arbitrarty false positive or false negative rates, however, is asemigroup. By??, the union of two approximate sets is also an approximate set with false positiveand false negative rates that depend upon the properties of the two approximate sets.However, as specified, this structure is not a monoid. To be amonoid, the set-union identityelement,∅, must be a member. The total functionfis (possibly) non-surjective, and thus there maybe no objective setAsuch thatf(A) =∅. Furthermore, even if∅∈image(f), other implementationsof approximate set generators may have a differentlabelfor the empty set.If a monoid is desired, a special case may be made for the∅so that it is always generatedby any implementation. This is trivially done by always mapping the empty set to the empty set,i.e.,f(∅) =∅. The probabilistic model is degenerate with respect to the empty set,∅∗=∅withprobability1.Note that theintersectionoperator overP(U)is also a commutative monoid with the identityelementU. Since∅≡ U, the universal setUisalsodegenerate, i.e.,U∗=Uwith probability1.Thus, with the degenerate case∅∗=∅, the approximate set overPUis also a commutative monoidwith the intersection operator. As monoids, we may use them in generic programming algorithmsthat have the constraint that the data type is monoidal.See??for a C++ implementation of the set-theoretic model for


-----
However, as specified, this structure is not a monoid. To be a \emph{monoid}, the set-union identity element, $\EmptySet$, must be a member. The total function $\operatorname{f}$ is (possibly) non-surjective, and thus there may be no objective set $\Set{A}$ such that $\operatorname{f}(\Set{A}) = \EmptySet$. Furthermore, even if $\EmptySet \in \Image(\operatorname{f})$, other implementations of approximate set generators may have a different \emph{label} for the empty set.

If a monoid is desired, a special case may be made for the $\EmptySet$ so that it is always generated by any implementation. This is trivially done by always mapping the empty set to the empty set, i.e., $\operatorname{f}(\EmptySet) = \EmptySet$. The probabilistic model is degenerate with respect to the empty set, $\ASet{\EmptySet} = \EmptySet$ with probability $1$.

Note that the \emph{intersection} operator over $\PowerSet(\Set{U})$ is also a commutative monoid with the identity element $\Set{U}$. Since $\SetComplement[\EmptySet] \equiv \Set{U}$, the universal set $\Set{U}$ is \emph{also} degenerate, i.e., $\ASet{U} = \Set{U}$ with probability $1$. Thus, with the degenerate case $\ASet{\EmptySet} = \EmptySet$, the approximate set over $\PowerSet{\Set{U}}$ is also a commutative monoid with the intersection operator. As monoids, we may use them in generic programming algorithms that have the constraint that the data type is monoidal.

[2019-11-16 03:41] this is a test

[2019-11-12 03:41] start with a measurable space (U,2^U) then in two parts, consider a measure space (U,sigma(generated),u) for the deterministic generator that doesn't cover 2^U, and the random model (U,2^U,P) where P is known as sums on P({x}) where x is an element of U.

[2019-11-12 03:41] note: more of this in What Makes Variables Random

[2019-11-26 10:49] concepts: set of axioms satisfied by a data type and a set of operations on it a concept.
this is what an approximate set is.

[2019-11-26 10:51] two countably infinite sets are equal if same representation.
if diff. representations, they are not equal.

[2019-11-26 10:51] this holds even if generator of asets is non-deterministic.

[2019-11-26 10:56] two finite approx sets are equal if same rep, but due to finiteness non-rep eq doesn't mean non-equality.
probabilistic. for large universal sets, this is minor.

[2019-11-26 10:56] new function: probability_models(A+,B) computes prob.
that A+ is a model of B (generated from B) given fpr and such.

[2019-11-26 10:58] equality is most function in computational basis of a data type, tells us that two objects of type T mean the same thing.
