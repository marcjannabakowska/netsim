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

using const_iterator = std::list<Package>::const_iterator;

class IPackageReceiver {
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;

    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

   virtual ~IPackageReceiver() = default;

};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<PackageQueue> d = std::make_unique<PackageQueue>(PackageQueue(FIFO)));
    void receive_package(Package&& p) override;
    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return STOREHOUSE;};
//
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator begin() const override {return d_->begin();}
    IPackageStockpile::const_iterator cend() const override { return d_->cend();}
    IPackageStockpile::const_iterator end() const override { return d_->end();}
    IPackageStockpile* get_queue() const {return d_.get();}
private:
    ElementID id_;
    std::unique_ptr<PackageQueue> d_;
};
//
//
//
class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator) : pg_(std::move(pg)) {};

    void add_receiver (IPackageReceiver* r);
    void remove_receiver (IPackageReceiver* r);
    IPackageReceiver *choose_receiver();
    preferences_t& get_preferences()  {return preferences_;};
    preferences_t& get_preferences() const { return const_cast<preferences_t&>(preferences_);}
    bool empty() const {return preferences_.empty();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator begin() const {return preferences_.begin();}
    const_iterator cend() const { return preferences_.cend();}
    const_iterator end() const { return preferences_.end();}
private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;

};

class PackageSender: public ReceiverPreferences {
public:


    ReceiverPreferences receiver_preferences_;
    void send_package();
    std::optional<Package> &get_sending_buffer() {return buffer_;};
    const std::optional<Package>& get_sending_buffer() const {return buffer_;};

protected:
    void push_package(Package&& p) { if (!buffer_) { buffer_.emplace(p); } };
    std::optional<Package> buffer_ = std::nullopt;

};



class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const {return di_;};
    ElementID get_id() const {return id_;}

private:
    ElementID id_;
    TimeOffset di_;

};



class Worker  : public IPackageReceiver, public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q);
    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return WORKER;};
    void do_work(Time t);
    void receive_package(Package&& p) override;
    TimeOffset get_processing_duration()  const {return pd_;};

    Time get_package_processing_start_time() const {return package_processing_start_time_;};
    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}
    IPackageStockpile::const_iterator begin() const override {return q_->begin();}
    IPackageStockpile::const_iterator cend() const override { return q_->cend();}
    IPackageStockpile::const_iterator end() const override { return q_->end();}
    IPackageQueue* get_queue() const {return q_.get();}
    const std::optional<Package>& get_package_processing_buffer() const {return buffer_processing_;}
private:
    std::optional<Package> buffer_processing_;
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<PackageQueue> q_;
    Time package_processing_start_time_;

};



#endif //NET_SIM_NODES_HPP
