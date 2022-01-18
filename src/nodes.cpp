////
//// Created by ZoJa on 14.12.2021.
////
#include "nodes.hpp"
//
//
//
//
Storehouse::Storehouse(ElementID id, std::unique_ptr<PackageQueue> d) {
    id_ = id;
    d_ = std::move(d);
}

void Storehouse::receive_package(Package&& p) {
        d_->push(std::move(p));

}

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    // Funkcja dodająca odbiorcę do kontenera preferences_t_ i zmieniająca wrtości dystrybuanty
    preferences_.emplace(r, 1);
//
    for (auto i = preferences_.begin(); i != preferences_.end(); i++)
    {
        i->second = 1 / (double)(preferences_.size()); // skaluje prawd. wylosowania odbiorcy, na tym etapie każdy będzie miał to samo prawd.
    }

}


IPackageReceiver *ReceiverPreferences::choose_receiver() {
//     Funkcja losująca wartość prawdopodobieństwa a następnie szukająca odbiorcy

    double dist = probability_generator();
    double sum_of_ps = 0;
    for (auto i = preferences_.begin(); i != preferences_.end(); i++)
    {
        sum_of_ps += i->second;
        if (sum_of_ps > dist) {
            return i->first;
        }
    }
    return nullptr;


}


void ReceiverPreferences::remove_receiver (IPackageReceiver* r) {
    preferences_.erase(r);
    if (preferences_.size() != 0) {
        for (auto i = preferences_.begin(); i != preferences_.end(); i++)
        {
            i->second = 1 / (double)(preferences_.size()); // skaluje prawd. wylosowania odbiorcy, na tym etapie każdy będzie miał to samo prawd.
        }
    }
}

void PackageSender::send_package() {
    if(buffer_){
        receiver_preferences_.choose_receiver()->receive_package(buffer_->get_id());
        buffer_.reset();
    }
}



Ramp::Ramp(ElementID id, TimeOffset di) {
    id_ = id;
    di_ = di;
}

void Ramp::deliver_goods(Time t)  {
    if (t%di_ == 0) {
        buffer_.reset();
    }
    else {
        Package x;
        buffer_.emplace(x);
    }
}

//
Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) {
    id_ = id;
    pd_ = pd;
    q_ = std::move(q);
}
//
void Worker::receive_package(Package&& p) {
//    PackageSender::push_package(std::move(p));
q_->push(std::move(p));
}



void Worker::do_work(Time t) {


    if(!q_->empty() and !buffer_processing_){
        buffer_processing_ = q_->pop();
        package_processing_start_time_ = t;
    }
    if(t - package_processing_start_time_+1>= pd_ and bool(buffer_processing_)){

        push_package(std::move(buffer_processing_.value()));
        buffer_processing_.reset();
        package_processing_start_time_ = 0;
    }


}