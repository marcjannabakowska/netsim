//
// Created by ZoJa on 14.12.2021.
//
#include "nodes.hpp"

Ramp::Ramp(ElementID id, TimeOffset di) {
    id_ = id;
    di_ = di;
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) {
    id_ = id;
    pd_ = pd;
    *q_ = *q;
}

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) {
    id_ = id;
    d_ = std::move(d);
}

