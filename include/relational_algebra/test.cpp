#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/vector/vector_fwd.hpp>
#include <boost/fusion/include/vector_fwd.hpp>
#include <string>
#include <iostream>

using namespace boost::fusion;

int main()
{
    vector<int, float> v(12, 5.5f);
    boost::fusion::at_c<1>(v);

    //std::cout << at_c<0>(v) << std::endl;
    //std::cout << at_c<1>(v) << std::endl;
}