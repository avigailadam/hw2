#include "unionFind.h"
int main() {
    UnionFind a1(10,10);
    a1.Union(1,2);
    std::cout<<a1;
    a1.Union(3,4);
    std::cout<<a1;
    a1.Union(2,5);
    std::cout<<a1;
    a1.Union(2,3);
    std::cout<<a1;
    a1.find(4);
    std::cout<<a1;
//    a1.Union(2,6);
//    a1.Union(2,7);
//    std::cout<<a1;
//    std::cout<<a1.find(5)<<std::endl;
//    a1.Union(1,3);
//    std::cout<<a1;
//    std::cout<<a1.find(3)<<std::endl;
//    std::cout<<a1.find(5)<<std::endl;
//    std::cout<<a1.find(9)<<std::endl;
    return 0;
}