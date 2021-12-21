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

    virtual std::size_t size() = 0;
    virtual bool empty() = 0;
    virtual void push(Package&& t) = 0;
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
    bool empty() override;
    void push(Package&& t) override {s_.push_back(t);};
    std::size_t size() override {return s_.size();}
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

};

class PackageQueue : public IPackageQueue
{
public:
    explicit PackageQueue(PackageQueueType queue_type): packageQueueType(queue_type) {};
    Package pop() override;
    PackageQueueType get_queue_type() override {return packageQueueType;};
private:
    PackageQueueType packageQueueType;
};




#endif //NET_SIM_STORAGE_TYPES_HPP