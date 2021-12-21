#include <iostream>
#include "storage_types.hpp"
#include "package.hpp"

int main() {
    std::cout<<"dupaa"<<std::endl;
    {
        Package p1;
    }
    Package p2;
    std::cout<<p2.get_id();

//    Package sto;
//    Package sto1;
//    std::cout<<sto.get_id()<<std::endl;
//    std::cout<<sto1.get_id()<<std::endl;
//    std::cout<<sto2.get_id()<<std::endl;
//    Package p1;
//    Package p2 = std::move(p1);
//    IPackageStockpile cd;

//    cd.push(sto.get_id());
//    cd.push(sto1.get_id());
//
//    for (auto el: cd) {
//    std::cout<<&el<<std::endl;

    return 0;
}
