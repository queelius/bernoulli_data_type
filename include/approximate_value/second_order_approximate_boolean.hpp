template <typename T, typename I>
struct bernoulli {};

//template <typename I>
struct bernoulli<bool,double>
{
    using value_type = bool;
    using error_type = double;
    
    value_type value;
    error_type err;

    /**
     * Let
     * 
     *     W_q = [X != q | Y = q] ~ bernoulli<bool>
     * 
     * be a Bernoulli approximation over Boolean values.
     * 
     * Since {0,1} only has two partitions, {{0,1}} and
     * {{0},{1]}}, there are only two possible orders to
     * bernoulli<bool>.
     * 
     * In the first-order Bernoulli model, we use the partition
     * {{0,1}} and P[W_q = 1] = r for q = 0 and q = 1 where r is
     * the error rate.
     * 
     * In a second-order Bernoulli model, we use the partition
     * {{0},{1}} such that over the first block {0} in the partition,
     * 
     *     P[W_0 = 1] = a
     * 
     * and over the second block {1}
     * 
     *     P[W_1 = 1] = b,
     * 
     * where a and b are respectively denoted the false positive and false
     * negative error rates. (In the first order model, the false
     * positive rate and false negative rates are equal to r.)
     * 
     * If we are given an observation of W_q and wish to determine
     * the probability that it is erroneous, i.e., given an
     * observed value W_q = 1, which is the probability that Y != x,
     * 
     *     P[X != x | Y == x].
     * 
     * 
     * We are given an observation of X rather than Y, but we are
     * interested in
     *     alpha := P[Y == 0|X == 0],
     * which is the 
     * if X realizes a value 0 and
     *     beta := P[Y == 1|X == 1].
     * 
     * By the laws of probability
     *     alpha := P[Y == 0|X == 0]
     * may be rewritten as
     *     alpha = P[Y == 0,X == 0] / P[X == 0]
     *           = P[X == 0|Y == 0] P[Y == 0] / P[X == 0]
     *           = (1-fpr(X)) P[Y == 0] / P[X == 0].
     * 
     * By the total law of probability,
     *     P[X == 0] = P[X == 0,Y == 0] + P[X == 0,Y != 0]
     *               = P[X == 0|Y == 0]P[Y == 0] + P[X == 0|Y != 0]P[Y != 0]
     *               = (1-fpr(X))P[Y == 0] + fnr(X)(1-P[Y == 0]).
     * 
     * If we denote P[Y == 0] by p and substitute P[X == 0] with the above,
     * we get the result
     * 
     *     alpha(p|X==0) =       p(1-fpr(X))
     *                     -----------------------.
     *                     p(1-fpr(X))+(1-p)fnr(X)
     * 
     * Then, of course, error(p|X==0) == 1-alpha(p|X==0), which is given by
     * 
     *     error(p|X==0) =       (1-p)fnr(X)
     *                     -----------------------
     *                     p(1-fpr(X))+(1-p)fnr(X)
     * 
     * By the laws of probability
     *     alpha := P[Y == 1|X == 1]
     * may be rewritten as
     *     alpha = P[Y == 1,X == 1] / P[X == 1]
     *           = P[X == 1|Y == 1] P[Y == 1] / P[X == 1]
     *           = (1-fnr(X)) P[Y == 1] / P[X == 1].
     * 
     * By the total law of probability,
     *     P[X == 1] = P[X == 1,Y == 0] + P[X == 1,Y != 0]
     *               = P[X == 1|Y == 0]P[Y == 0] + P[X == 1|Y != 0]P[Y != 0]
     *               = fpr(X) P[Y == 0] + (1-fnr(X))(1-P[Y == 0]).
     * 
     * If we denote P[Y == 0] by p and substitute P[X == 0] with the above,
     * we get the result
     *     alpha(p|X==1) =       p(1-fnr(X))
     *                     -----------------------.
     *                     p(1-fpr(X))+(1-p)fnr(X)
     
     */
    double alpha(double p)
    {
        if (value)
        {
            
        }
        else
        {
            return p*(1-fpr) / (p*(1-fpr) + (1-p)*fnr);
        }        
    }

    auto operator()() const { return value; }
};

auto operator~(approximate<2,bool> a)
{
    return approximate<2,bool>{!a.value,a.fnr,a.fpr};
}



