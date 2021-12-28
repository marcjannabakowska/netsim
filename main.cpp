#include <iostream>
#include "storage_types.hpp"
#include "package.hpp"

int main() {
//    std::cout<<"dupaa"<<std::endl;
//    {
//        Package p1;
//    }
//    Package p2;
//    std::cout<<p2.get_id();

    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));
    std::cout<<q.begin()->get_id();
    Package p(std::move(q.pop()));
    std::cout<<p.get_id()<<std::endl;
    p = q.pop();
    std::cout<<p.get_id();
////    PackageQueue q(PackageQueueType::LIFO);
//    q.push(Package(1));
//    q.push(Package(2));
//
//    Package p(std::move(q.pop()));




    return 0;
}
