

/**
 * The expected error rate, denoted by alpha,
 * for the equality predicate is given by
 *     Pr[approximate_value<T>{T(t)} != T(t)] = alpha,
 * where the value T(t) is uncertain (a latent
 * variable) and approximate_value<T>{T(t)} is
 * an observable quantity.
 *
 * If we are given T(t), then the probability
 * that approximate_value<T>{T(t)} takes on
 * a specific value is given by the nature
 * of the random approximate model. For instance,
 * a second-order random approximate Boolean
 * is 
 *
 *
 * There is usually much more structure
 * in the distribution of approximate values.
 * At a more fine-grained level, we may be
 * interested in
 *     Pr[T = a | T' = approximate_value<T>(x)].
 *
 * We can reduce this to the 
 * although the random approximate value type
 * model does not make any
 * even though most random approximate
 * value types have more complicated structure,
 * e.g., an entropy map generates
 * approximate maps in which each value mapped
 * to is a function of the implicit probability
 * as given by the entropy coder.
 */

template <int N, typename T>
struct approximate {};


template <typename T>
struct approximate<0,T>
{
    using value_type = T;
    auto constexpr order = 0;

    T value;

    double error(double alpha) const { return 0; }

    auto operator()() const { return value; }
};

