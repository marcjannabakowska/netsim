//
// Created by ZoJa on 14.12.2021.
//


#include "reports.hpp"
void generate_structure_report(const Factory& f, std::ostream& os){
    os<<"\n== LOADING RAMPS ==\n\n";
    std::set<ElementID> workers;
    std::set<ElementID> stores;
    for(auto i = f.ramp_cbegin();i!= f.ramp_cend();i++){
        os<<"LOADING RAMP #"<<std::to_string(i->get_id())<<"\n  Delivery interval: "<<std::to_string(i->get_delivery_interval())<<"\n  Receivers:\n";
        for(auto it = i->receiver_preferences_.cbegin(); it != i->receiver_preferences_.cend();it++){
            if(it->first->get_receiver_type() == ReceiverType::WORKER){
                workers.insert(it->first->get_id());
            }else if(it->first->get_receiver_type() == ReceiverType::STOREHOUSE){
                stores.insert(it->first->get_id());
            }
        }
        for(auto j : workers){
            os<<"    worker #"<<std::to_string(j)<<"\n";
        }
        for(auto j : stores){
            os<<"    storehouse #"<<std::to_string(j)<<"\n";
        }
        os<<"\n";
    }
    workers.clear();
    stores.clear();
    os<<"\n== WORKERS ==\n\n";
    std::string queue_type;
    for(auto i = f.worker_cbegin();i!= f.worker_cend();i++){
        switch (i->get_queue()->get_queue_type()){
            case PackageQueueType::LIFO:{
                queue_type = "LIFO";
                break;
            }
            case PackageQueueType::FIFO:{
                queue_type = "FIFO";
                break;
            }
            default:
                break;
        }
        os<<"WORKER #"<<std::to_string(i->get_id())<<"\n  Processing time: "<<std::to_string(i->get_processing_duration())<<"\n  Queue type: "<<queue_type<<"\n  Receivers:\n";
        for(auto it = i->receiver_preferences_.cbegin(); it != i->receiver_preferences_.cend();it++){
            if(it->first->get_receiver_type() == ReceiverType::WORKER){
                workers.insert(it->first->get_id());
            }else if(it->first->get_receiver_type() == ReceiverType::STOREHOUSE){
                stores.insert(it->first->get_id());
            }
        }
        for(auto j : workers){
            os<<"    worker #"<<std::to_string(j)<<"\n";
        }
        for(auto j : stores){
            os<<"    storehouse #"<<std::to_string(j)<<"\n";
        }
        os<<"\n";
    }
    os<<"\n== STOREHOUSES ==\n\n";
    for(auto i = f.storehouse_cbegin();i!= f.storehouse_cend();i++){
        os<<"STOREHOUSE #"<<std::to_string(i->get_id())<<"\n\n";
    }
}
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os<<"=== [ Turn: "<< std::to_string(t)<<" ] ===\n\n";

    os<<"== WORKERS ==\n";
    if(f.worker_cend()!=f.worker_cbegin()){
        os<<"\n";
    }
    std::set<ElementID> workers;
    for(auto i = f.worker_cbegin();i!= f.worker_cend();i++){
        workers.insert(i->get_id());
    }
    for(auto i:workers){
        auto it = f.find_worker_by_id(i);
        os<<"WORKER #"<<std::to_string(i)<<"\n";
        ///PBuffer
        if(bool(it->get_package_processing_buffer())){
            os<<"  PBuffer: #"<<it->get_package_processing_buffer()->get_id()<<" (pt = "<<std::to_string(t-it->get_package_processing_start_time()+1)<<")\n";
        }else{
            os<<"  PBuffer: (empty)\n";
        }
        ///Queue
        if(it->get_queue()->empty()){
            os<<"  Queue: (empty)\n";
        }else{
            os<<"  Queue:";
            for(auto j = it->get_queue()->cbegin(); j!= it->get_queue()->cend() ; j++){
                if(j == it->get_queue()->cbegin()){
                    os<<" #"<<j->get_id();
                }else{
                    os<<", #"<<j->get_id();
                }
            }
            os<<"\n";
        }
        ///SBuffer
        if(bool(it->get_sending_buffer())){
            os<<"  SBuffer: #"<<it->get_sending_buffer()->get_id()<<"\n";
        }else{
            os<<"  SBuffer: (empty)\n";
        }

    }
    os<<"\n\n== STOREHOUSES ==\n\n";
    std::set<ElementID> stores;
    for(auto i = f.storehouse_cbegin();i!= f.storehouse_cend();i++){
        stores.insert(i->get_id());
    }
    for(auto i:stores){
        auto it = f.find_storehouse_by_id(i);
        os<<"STOREHOUSE #"<<std::to_string(i)<<"\n";
        ///Queue
        if(it->get_queue()->empty()){
            os<<"  Stock: (empty)\n\n";
        }else{
            os<<"  Stock:";
            for(auto j = it->get_queue()->cbegin(); j!= it->get_queue()->cend() ; j++){
                if(j == it->get_queue()->cbegin()){
                    os<<" #"<<j->get_id();
                }else{
                    os<<", #"<<j->get_id();
                }
            }
            os<<"\n\n";
        }
    }

}