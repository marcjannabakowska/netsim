//
// Created by ZoJa on 14.12.2021.
//

#include "storage_types.hpp"

bool IPackageStockpile::empty() {
    if (s_.size() != 0)
    {
        return false;
    }
    else {
        return true;
    }
}

