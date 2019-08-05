#include <optional>
#include <iostream>
#include <variant>
#include <algorithm>


template <
    typename R
>
class ApproximateBags
{
public:
    // value_type should be some sort of pair type
    // where each element is of the same type,
    // otherwise symmetry should not be possible.
    using value_type = typename R::value_type;

    ApproximateBags(size_t max_bag)
    {

    };

    // we enforce the invariant that if (x1, ..., xn) in R
    // then all permutations of x1, ..., xn are in the bag.
    template <typename Bag>
    bool contains(Bag const & xs) const
    {
        for (auto const p : permutations(xs))
        {
            if (contains(p))
                return true;
        }
        return false;
    };

    auto fpr() const { return 1. - std::pow(1 - _r.fpr(), 2.); };

    // if R is a random approximate set, then the
    // true positive rate with respect to the
    // modified contains function still has the
    // same true positive rate.
    auto tpr() const { return _r.tpr(); };

private:
    R _r;
};





/**
 * A data type T models the concept of a random approximate map if
 * ...
 */


class F
{
public:
    using domain = int;
    using codomain = double;

    double fpr() const { return 0.2; };
    double tpr() const { return 0.9; };

    std::optional<codomain> operator()(domain const & x) const { return 2*x; };
};

int main()
{
    F f;

    std::cout << "f.fpr: " << f.fpr() << std::endl;
    std::cout << "f.tpr: " << f.tpr() << std::endl;
    std::cout << "---\n";
    auto c1 = compose(Identity<double>(),f);
    std::cout << "c1.fpr: " << c1.fpr().first << "," << c1.fpr().second << std::endl;
    std::cout << "c1.tpr: " << c1.tpr() << std::endl;    
    std::cout << "---\n";
    
    /*auto c2 = compose(f,Identity<int>());

    std::cout << "c1.fpr: " << c1.fpr() << std::endl;
    std::cout << "c1.tpr: " << c1.tpr() << std::endl;

    std::cout << "c2.fpr: " << c2.fpr() << std::endl;
    std::cout << "c2.tpr: " << c2.tpr() << std::endl;*/
}