struct approximate<2,bool>
{
    using value_type = bool;
    auto constexpr order = 2;

    bool value;
    double fpr;
    double fnr;

    /**
     * Suppose
     *     X ~ random_approximate<2,bool> and
     *     Y ~ random_approximate<0,bool>.
     * Then, by the first-order random approximate Boolean model,
     *     P[X != 0|Y == 0] == fpr(X) and
     *     P[X != 1|Y == 1] == fnr(X). 
     * 
     * We are given an observation of X rather than Y, but we are
     * interested in
     *     alpha := P[Y == 0|X == 0]
     * if X realizes a value 0 and
     *     alpha := P[Y == 1|X == 1].
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
     *     alpha(p|X==0) =       p(1-fpr(X))
     *                     -----------------------.
     *                     p(1-fpr(X))+(1-p)fnr(X)
     * 
     * Then, of course, error(p|X==0) == 1-alpha(p|X==0), which is given by
     *     error(p|X==0) = (1-p)fnr(X)
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



