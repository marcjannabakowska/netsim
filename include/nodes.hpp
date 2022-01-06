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
    virtual IPackageStockpile::const_iterator begin()  = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator end() = 0;
};


class Storehouse : public IPackageReceiver {
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<IPackageStockpile>(PackageQueue(FIFO)));

    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return receiver_type_;};
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
    ReceiverType receiver_type_;
};


class Worker : public IPackageReceiver {
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return receiver_type_;};
    void do_work(Time t);
    TimeOffset get_processing_duration() {return pd_;};
    Time get_package_processing_start_time();

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    ReceiverType receiver_type_;
};

class ReceiverPreferences {
    using preferences_t = std::map<IPackageReceiver*, double>;
    ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator) : pg_(std::move(pg)) {}; //chyba tak powinien wyglądać ten konstruktor domyślny

    void add_receiver (IPackageReceiver* r);
    void remove_receiver (IPackageReceiver* r);
    IPackageReceiver choose_receiver ();
    preferences_t& get_preferences();
private:
    ProbabilityGenerator pg_;

};

class PackageSender {
    static ReceiverPreferences receiver_preferences_;
    void send_package();
    std::optional<Package>& get_sending_buffer();

protected:
    void push_package(Package&& p);



};



class Ramp {
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t); //TODO
    TimeOffset get_delivery_interval() const {return di_;};
    ElementID get_id() const {return id_;}
private:
    ElementID id_;
    TimeOffset di_;

};




#endif //NET_SIM_NODES_HPP
