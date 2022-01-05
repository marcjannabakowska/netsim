//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_NODES_HPP
#define NET_SIM_NODES_HPP

#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include <optional>
class IPackageReceiver {
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() = 0; // czy to nie powinna byc metoda z package

};

class ReceiverPreferences {
   // static preferences_t; //FIXME: jak to zadeklarowac
   ReceiverPreferences(ProbabilityGenerator pg); //jak to ma wygladac z tym konstruktorem
   void add_receiver (IPackageReceiver* r);
   void remove_receiver (IPackageReceiver* r);
   IPackageReceiver choose_receiver ();
   //preferences_t& get_preferences(); co z tym bo nie wiem jak statica zrobić

};

class PackageSender {
    static ReceiverPreferences receiver_preferences_; //jak sie robi statyczne
    void send_package();
    std::optional<Package>& get_sending_buffer();

protected:
    void push_package(Package&& p);



};



class Ramp {
    Ramp();
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval();
    ElementID get_id(); // czy to ma przejsc z package czy nie
};


class Worker : public IPackageReceiver {
    //jakiś konstruktor
    void do_work(Time t);
    TimeOffset get_processing_duration();
    Time get_package_processing_start_time();

};

class Storehouse : public IPackageReceiver {
    //konstruktor
};


#endif //NET_SIM_NODES_HPP
