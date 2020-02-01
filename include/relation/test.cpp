

#include <iostream>
#include "binary_relation.hpp"

int main()
{
    auto r = new alex::relation::mutable_binary_relation<int,char>;

    alex::relation::mutable_binary_relation<int,char> r1;
    alex::relation::mutable_binary_relation<int,char> r2;

    r1.add(2, 'a');
    r1.add(2, 'b');

    r1.add(3, 'a');

    r1.add(4, 'a');
    r1.add(4, 'c');

    r2.add(2, 'a');
    r2.add(2, 'b');
    r2.add(2, 'c');
    r2.add(2, 'd');
    r2.add(2, 'e');
    r2.add(2, 'f');



    r->add(2, 'a');
    r->add(2, 'b');
    r->add(2, 'c');
    r->add(2, 'd');
    r->add(2, 'e');
    r->add(2, 'f');

    r->add(3, 'a');

    r->add(4, 'a');
    r->add(4, 'c');

//    for (auto [x,y] : r)
//    {
//        std::cout << "(" << x << ", " << y << ")\n";
//    }

 //   auto r2 = r.image(2);

    //for (auto x : r2)
   // {
   //     std::cout << x << "\n";
   // }

    std::cout << "---\n";
    std::cout << r->operator()(2, 'a') << "\n";
    std::cout << r->operator()(2, 'g') << "\n";
    std::cout << "---\n";

    auto rexp = static_cast<alex::relation::binary_relation_expr<alex::relation::mutable_binary_relation<int,char>,int,char>*>(r);

    auto p = std::make_pair(2,'a');
    std::cout << rexp->operator()(p) << "\n";
    std::cout << rexp->operator()(2, 'g') << "\n";
    std::cout << rexp->size() << "\n";

    std::cout << "bin rel\n";
    auto u2 = alex::relation::binary_relation(r1);


    auto u2begin = u2.cbegin();
    //auto u2end = u2.cend();

    int j = 0;


    while (true)
    {
        u2begin++;
        ++j;
        if (j == 3)
            break;

    }

    std::cout << "bin rel end\n";

    //for (auto const & xx : u2)
   // {
   //     std::cout << xx.first << ", " << xx.second << "\n";
   // }
    

    //alex::relation::mutable_binary_relation<int,char> u = alex::relation::binary_relation_union(r1,r2);
    //std::cout << u(p) << "\n";
    //std::cout << u(2, 'a') << "\n";
    //std::cout << u(3, 'a') << "\n";


}