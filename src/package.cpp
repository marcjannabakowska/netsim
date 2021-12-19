//
// Created by ZoJa on 14.12.2021.
//
#include "package.hpp"



Package::Package() {
    if (!freed_IDs_.empty()) {
        id = *(freed_IDs_.begin());
        assigned_IDs_.insert(*(freed_IDs_.begin()));    /* dodanie nowego id  do zbioru przypsanych */
        freed_IDs_.erase(freed_IDs_.begin());    /* usunięcie przypisanego ze zbioru wolnych */
    } else if (!assigned_IDs_.empty()) {
        id = *(assigned_IDs_.end())++;        /* inkrementacja */
        assigned_IDs_.insert(*(assigned_IDs_.end())++);
    }

}

bool Package::operator==(const Package& p) {
    return this->id == p.id;
}

Package::Package(const ElementID ID) {
    id = ID;
    assigned_IDs_.insert(ID);
}

Package::~Package() {
    assigned_IDs_.erase(assigned_IDs_.find(id));
    freed_IDs_.insert(id);   // dodajemy do wolnych te usunięte, aby mogły być znowu użyte

}