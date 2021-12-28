//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_STORAGE_TYPES_HPP
#define NET_SIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include<list>


enum PackageQueueType {FIFO, LIFO};
class IPackageStockpile {
public:
//    IPackageStockpile(ElementID s) : s_(s) {} //usuwam konstruktor

    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&& t) = 0;
    virtual std::size_t size() = 0;
    virtual bool empty() = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator begin()  = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator end() = 0;

    virtual ~IPackageStockpile() = default;

//protected:
//    std::list<Package> s_;
//skutkiem usuniecia konstruktora jest usuniecie tych metod protected
};


class IPackageQueue : public IPackageStockpile
{
protected:
    PackageQueueType queue_type;

public:
    //wczesniej rozwiniecia metody empty było tutaj ale wrzuciłam poniżej
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

};

class PackageQueue : public IPackageQueue
{
public:
    bool empty() override;
    PackageQueue(PackageQueueType queue_type): q_type(queue_type) {};
    Package pop() override;
    void push(Package&& t) override {s_.push_back(t);};
    std::size_t size() override {return s_.size();}
    PackageQueueType get_queue_type() override {return q_type;};
    IPackageStockpile::const_iterator cbegin() const override {return s_.cbegin();}
    IPackageStockpile::const_iterator begin() override {return s_.begin();}
    IPackageStockpile::const_iterator cend() const override { return s_.cend();}
    IPackageStockpile::const_iterator end() override { return s_.end();}
private:
    PackageQueueType q_type;
    std::list<Package> s_;
};



#endif //NET_SIM_STORAGE_TYPES_HPP