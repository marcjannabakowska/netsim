//
// Created by ZoJa on 14.12.2021.
//

#include "factory.hpp"


bool has_reachable_storehouse(/* const */PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if(node_colors[sender] == NodeColor::VERIFIED){
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;
    if(sender->receiver_preferences_.get_preferences().empty()){
        throw std::logic_error("Sender has not got any receivers");
    }
    bool has_sender_at_least_one_receiver_other_than_himself = false;
    for(auto receiver : sender->receiver_preferences_.get_preferences()){
        if(receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE){
            has_sender_at_least_one_receiver_other_than_himself = true;
        }
        else if(receiver.first->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<class Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender){
                continue;
            }
            has_sender_at_least_one_receiver_other_than_himself = true;
            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                has_reachable_storehouse(sendrecv_ptr,node_colors);
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if(has_sender_at_least_one_receiver_other_than_himself){
        return true;
    }
    else{
        throw std::logic_error("Error");
    }
}

void Factory::do_deliveries(Time t) {
    for (auto &ramp : cont_r) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_work(Time t ) {
    for (auto& worker : cont_w)
        worker.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &ramp : cont_r) {
        ramp.send_package();
    }
    for (auto &worker : cont_w) {
        worker.send_package();
    }
}

bool Factory::is_consistent() {
    std::map<const PackageSender *, NodeColor> kolor;
    for (auto ptr = cont_w.begin(); ptr != cont_w.end(); ptr++){
        Worker* worker = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(worker);
        kolor[sender] = NodeColor::UNVISITED;
    }
    for (auto ptr = cont_r.begin(); ptr != cont_r.end(); ptr++){
        Ramp* ramp = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(ramp);
        kolor[sender] = NodeColor::UNVISITED;
    }
    try {
        for (auto ptr = cont_r.begin(); ptr != cont_r.end(); ptr++){
            Ramp* ramp = &(*ptr);
            auto sender = dynamic_cast<PackageSender*>(ramp);
            has_reachable_storehouse(sender, kolor);
        }
    }
    catch (const std::logic_error&) {
        return false;
    }
    return true;
}

