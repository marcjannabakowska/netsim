//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_NODES_HPP
#define NET_SIM_NODES_HPP

#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include <optional>
#include <iostream>
#include <memory>
#include <map>
#include <utility>
#include "helpers.hpp"

enum ReceiverType {WORKER, STOREHOUSE};

class IPackageReceiver {
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

    //virtual ~IPackageReceiver() = default;

protected:

    std::list<Package> package_queue_;
};



class Storehouse : public IPackageReceiver {
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<IPackageStockpile>(PackageQueue(FIFO)));

    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return STOREHOUSE;};
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};



class ReceiverPreferences {
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator) : pg_(std::move(pg)) {}; //chyba tak powinien wyglądać ten konstruktor domyślny

    const_iterator cbegin() const {return preferences_.cbegin();};
    const_iterator begin() const {return preferences_.begin();};
    const_iterator cend() const {return preferences_.cend();};
    const_iterator end() const {return preferences_.end();};


    void add_receiver (IPackageReceiver* r);
    void remove_receiver (IPackageReceiver* r);
    IPackageReceiver *choose_receiver();
    preferences_t& get_preferences();  //TODO: zaimplementować;
private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;

};

class PackageSender: ReceiverPreferences {
    static ReceiverPreferences receiver_preferences_;
    void send_package();
    std::optional<Package>& get_sending_buffer();


protected:
    void push_package(Package&& p);
    std::optional<Package> buffer_ = std::nullopt;


    PackageSender();
};



class Ramp : PackageSender {
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t); //TODO
    TimeOffset get_delivery_interval() const {return di_;};
    ElementID get_id() const {return id_;}
    std::optional<Package> &get_sending_buffer();
private:
    ElementID id_;
    TimeOffset di_;

};

class Worker : public IPackageReceiver, public PackageSender {
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return WORKER;};
    void do_work(Time t);
    TimeOffset get_processing_duration() {return pd_;};
    Time get_package_processing_start_time();

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    ReceiverType receiver_type_;
};



#endif //NET_SIM_NODES_HPP
