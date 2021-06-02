template <typename L, typename T>
struct bernoulli {};

struct bernoulli<struct first_order, bool>
{
    using value_type = bool;

    bool value;
    double error;
};

struct bernoulli<struct second_order, bool>
{
    using value_type = bool;

    bool value;
    double fpr, fnr;
};

struct bernoulli<struct higher_order, bool>
{
    using value_type = bool;

    bool value;
    interval<double> fpr, fnr;
};


auto fpr(bernoulli<struct first_order,bool> const & x)
{
    return x.error;
}

auto fnr(bernoulli<struct first_order,bool> const & x)
{
    return x.error;
}

auto tpr(bernoulli<struct first_order,bool> const & x)
{
    return 1-x.error;
}

auto tnr(bernoulli<struct first_order,bool> const & x)
{
    return 1-x.error;
}

auto fpr(bernoulli<struct second_order,bool> const & x)
{
    return x.fpr;
}

auto fnr(bernoulli<struct second_order,bool> const & x)
{
    return x.fnr;
}

auto tpr(bernoulli<struct second_order,bool> const & x)
{
    return 1-fnr(x);
}

auto tnr(bernoulli<struct second_order,bool> const & x)
{
    return 1-fpr(x);
}

auto fpr(bernoulli<struct higher_order,bool> const & x)
{
    return x.fpr;
}

auto fnr(bernoulli<struct higher_order,bool> const & x)
{
    return x.fnr;
}

auto operator~(bernoulli<struct first_order,bool> const & x)
{
    return bernoulli<struct first_order,bool>{!x.value,x.error};
}

auto operator~(bernoulli<struct second_order,bool> const & x)
{
    return bernoulli<struct second_order,bool>{!x.value,x.fnr,x.fpr};
}

auto operator~(bernoulli<struct higher_order,bool> const & x)
{
    return bernoulli<struct higher_order,bool>{!x.value,x.fnr,x.fpr};
}

auto operator&&(
    bernoulli<struct first_order,bool> const & x,
    bernoulli<struct first_order,bool> const & y)
{
    // three ways an error may occur when true value is 1
    auto const err1 = x.error[0] * (1 - y.error[0]);
    auto const err2 = y.error[0] * (1 - x.error[0]);
    auto const err3 = x.error[0] * y.error[0];

    // one way an error may occur when true value is 0
    auto const err4 = 1-(1-x.error[0])*(1-y.error[0]);

    // the result is bernoulli<4,bool> type. this may seem odd, since there
    // are only two values, {true, false}, and thus only 2 ways to partition
    // the set. however, this is coming from a binary operator on two
    // first-order boolean values. thus, the partition is over the input domain,
    // which has many more partitions.
    return bernoulli<struct higher_order,bool>{
        x.value && y.value,
        {err1,err2,err3,err4}
    };
}

auto operator&&(
    bernoulli<struct second_order,bool> const & x,
    bernoulli<struct second_order,bool> const & y)
{
    // three ways an error may occur when true value is 1
    auto const err1 = fnr(x) * tnr(y);
    auto const err2 = tnr(x) * fnr(y);
    auto const err3 = fpr(x) * fpr(y);

    // one way an error may occur when true value is 0
    auto const err4 = 1-tnr(x)*tnr(y);

    // the result is bernoulli<4,bool> type. this may seem odd, since there
    // are only two values, {true, false}, and thus only 2 ways to partition
    // the set. however, this is coming from a binary operator on two
    // first-order boolean values. thus, the partition is over the input domain,
    // which has many more partitions.
    return bernoulli<struct higher_order,bool>{
        x.value && y.value,
        {err1,err2,err3,err4}
    };
}


