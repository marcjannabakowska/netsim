//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_STORAGE_TYPES_HPP
#define NET_SIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include<list>



class IPackageStockpile {
public:
    IPackageStockpile(std::list<Package> s = {}) : s_(s) {}

    using const_iterator = std::list<Package>::const_iterator;

    std::size_t size() const {return s_.size();}
    bool empty();
    void push(Package&& t) {s_.push_back(t);}
    const_iterator cbegin() const {return s_.cbegin();}
    const_iterator begin() {return s_.begin();}
    const_iterator cend() const { return s_.cend();}
    const_iterator end() { return s_.end();}

    virtual ~IPackageStockpile() = default;
protected:            // zmieniam z private na protected!
    std::list<Package> s_;
};

enum class PackageQueueType
{
    FIFO,
    LIFO
};

class IPackageQueue : public IPackageStockpile
{
protected:
    PackageQueueType queue_type;
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
};

class PackageQueue : public IPackageQueue
{
public:
    explicit PackageQueue(PackageQueueType queue_type);
    Package pop();
};




#endif //NET_SIM_STORAGE_TYPES_HPP