//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_PACKAGE_HPP
#define NET_SIM_PACKAGE_HPP
#include <set>
#include "types.hpp"


class Package
{
public:
    Package();
    Package(ElementID ID);

    bool operator==(const Package& p);
    ElementID get_id() const { return id; }

    ~Package();


protected:
    static std::set<ElementID> freed_IDs_;
    static std::set<ElementID> assigned_IDs_;
    ElementID id;

};


#endif //NET_SIM_PACKAGE_HPP
