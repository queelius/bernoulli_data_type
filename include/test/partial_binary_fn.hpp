#include <memory>
#include <utility>
#include <tuple>
#include <optional>

using std::pair;
using std::optional;
using std::shared_ptr;
using std::tuple;

template <typename X, typename Y, typename Z>
class partial_binary_fn
{
public:
    using domain = pair<X,Y>;
    using codomain = Z;
    using relation = tuple<X,Y,Z>;

    optional<Z> operator()(X x, Y y) const
    {
        return fn_->operator()(x, y);
    }

private:
    struct concept
    {
        virtual optional<Z> operator()(X x, Y y) const = 0;
        virtual set_view<domain> dod() const = 0;
    };

    template <typename F>
    struct model: public concept
    {
        optional<Z> operator()(X x, Y y) const
        {
            return f(x, y);
        }

        set_view<domain> dod() const
        {
            return dod(f);
        }

        set_view<domain> image() const
        {
            return dod(f);
        }


        F f;
    };

    shared_ptr<concept> const fn_;
}





// (X, Y) -> 2^Z
template <typename X, typename Y, typename Z>
struct partial_binary_fn_union
{
    partial_binary_fn_union(
        partial_binary_fn<X,Y,Z> f1,
        partial_binary_fn<X,Y,Z> f2)
    {

    }

    
};





