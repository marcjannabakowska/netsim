#include <iostream>
#include "storage_types.hpp"
#include "package.hpp"

int main() {
    std::cout<<"dupaa"<<std::endl;
    Package sto;
    Package sto1;
    Package sto2;
//    std::cout<<sto.get_id()<<std::endl;
//    std::cout<<sto1.get_id()<<std::endl;
//    std::cout<<sto2.get_id()<<std::endl;

    std::list<Package> cd = {sto,sto1};
    IPackageStockpile x (cd);
//    std::cout<<x.size();
//    for (size_t i = 0; i < x.size(); i++) {
//        std::cout<<&x<<std::endl;
//    }
    return 0;
}
