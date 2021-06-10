struct bernoulli<second_order<pos>,bool,eq>
{
    using value_type = bool;
    using op = eq;
    using partition = second_order<pos>;

    auto error(bool x) const { return x ? 0. : fpr; }

    bool value;
    double fpr;
};

struct bernoulli<second_order<neg>,bool,eq>
{
    using value_type = bool;
    using op = eq;
    using partition = first_order;

    auto error(bool x) const { return x ? fnr : 0.; }

    bool value;
    double fnr;
};


struct bernoulli<second_order<pos_neg>,bool,eq>
{
    using value_type = bool;
    using op = eq;
    using partition = first_order;

    auto error(bool x) const { return x ? fnr : fpr; }

    bool value;
    double fpr, fnr;
};

template <typename T>
struct bernoulli<higher_order<T>,bool,eq>
{
    using value_type = bool;
    using op = eq;
    using partition = first_order;

    bool value;
    std::pair<double,double> fpr, fnr;
};


auto fpr(bernoulli<first_order,bool,eq> const & x)
{
    return x.err;
}

auto fnr(bernoulli<first_order,bool,eq> const & x)
{
    return x.err;
}

auto tpr(bernoulli<first_order,bool,eq> const & x)
{
    return 1-x.err;
}

auto tnr(bernoulli<first_order,bool,eq> const & x)
{
    return 1-x.err;
}

auto fpr(bernoulli<second_order<pos_neg>,bool,eq> const & x)
{
    return x.fpr;
}

auto fnr(bernoulli<second_order<pos_neg>,bool,eq> const & x)
{
    return x.fnr;
}

auto tpr(bernoulli<second_order<pos_neg>,bool,eq> const & x)
{
    return 1-fnr(x);
}

template <typename T>
auto fpr(bernoulli<higher_order<T>,bool,eq> const & x)
{
    return x.fpr;
}

auto tnr(bernoulli<second_order<pos_neg>,bool,eq> const & x)
{
    return 1-fpr(x);
}

template <typename T>
auto fnr(bernoulli<higher_order<T>,bool,eq> const & x)
{
    return x.fnr;
}

auto operator~(bernoulli<first_order,bool,eq> const & x)
{
    return bernoulli<first_order,bool,eq>{!x.value,x.err};
}

auto operator~(bernoulli<second_order<pos_neg>,bool,eq> const & x)
{
    return bernoulli<second_order<pos_neg>,bool,eq>{!x.value,x.fnr,x.fpr};
}

template <typename T>
auto operator~(bernoulli<higher_order<pos_neg>,bool,eq> const & x)
{
    return bernoulli<higher_order<T>,bool,eq>{!x.value,x.fnr,x.fpr};
}

/*
auto operator&&(
    bernoulli<first_order,bool,eq> const & x,
    bernoulli<first_order,bool,eq> const & y)
{
    std::pair<double,double> fprate;
    fprate.first = std::min(
            fnr(x)*tnr(y),
            std::min(tnr(x)*fnr(y), fpr(x)*fpr(y)));
    fprate.second = std::max(
            fnr(x)*tnr(y),
            std::max(tnr(x)*fnr(y), fpr(x)*fpr(y)));

    // three ways an error may occur when true value is 1
    auto const err1 = x.err * (1 - y.err);
    auto const err2 = y.err * (1 - x.err);
    auto const err3 = x.err * y.err;

    // one way an error may occur when true value is 0
    auto const err4 = 1-(1-x.err)*(1-y.err);

    // the result is bernoulli<4,bool> type. this may seem odd, since there
    // are only two values, {true, false}, and thus only 2 ways to partition
    // the set. however, this is coming from a binary operator on two
    // first-order boolean values. thus, the partition is over the input domain,
    // which has many more partitions.
    return bernoulli<higher_order<pos_neg>,bool,eq>{
        x.value && y.value,
        {err1,err2,err3,err4}
    };
}
*/


/**
 * We should only need one of these.
 */
template <typename T>
auto operator&&(
    bernoulli<T,bool,eq> const & x,
    bernoulli<T,bool,eq> const & y)
{
    auto fpr = min_span(min_span(fnr(x)*tnr(y), tnr(x)*fnr(y)), fpr(x)*fpr(y));
    auto fnr = min_span(1-tnr(x)*tnr(y))

    // the result is bernoulli<4,bool> type. this may seem odd, since there
    // are only two values, {true, false}, and thus only 2 ways to partition
    // the set. however, this is coming from a binary operator on two
    // first-order boolean values. thus, the partition is over the input domain,
    // which has many more partitions.
    return bernoulli<higher_order<pos_neg>,bool,eq>{
        x.value && y.value, fpr, fnr};
    };
}


