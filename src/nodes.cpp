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
    if (rec_type == STOREHOUSE) {
        d_->push(std::move(p));
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    // Funkcja dodająca odbiorcę do kontenera preferences_t_ i zmieniająca wrtości dystrybuanty


    if (preferences_.size() == 0) {
        preferences_[r] = 1;
    }

    else {
        preferences_[r] = pg_();
        double sum_of_ps = 0;
        for (auto [receiver, p] : preferences_) {
            sum_of_ps += p;
        }
        for (auto [receiver, p] : preferences_) {
            p = p/sum_of_ps;
        }
    }
}
//

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    // Funkcja losująca wartość prawdopodobieństwa a następnie szukająca odbiorcy

    ProbabilityGenerator dist = pg_;

    double sum_of_ps = 0;
    for (auto[receiver, p] : preferences_) {
        sum_of_ps += p;
        if (sum_of_ps >= dist())
            return receiver;
    }
    return nullptr;
}

void ReceiverPreferences::remove_receiver (IPackageReceiver* r) {
    preferences_.erase(r);
    if (preferences_.size() != 0) {
        double sum_pf_ps = 0;
        for (auto [receiver, p] : preferences_) {
            sum_pf_ps += p;
        }
        for (auto [receiver, p] : preferences_) {
            p = p/sum_pf_ps;
        }
    }
}

void PackageSender::send_package() {
    if(buffer_){
        receiver_preferences_.choose_receiver()->receive_package(buffer_->get_id());
    }
}
Ramp::Ramp(ElementID id, TimeOffset di) {
    id_ = id;
    di_ = di;
}

void Ramp::deliver_goods(Time t)  {
    if (t%di_ == 0) {
        Package x;
        buffer_.emplace(x);
    }
}


Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) {
    id_ = id;
    pd_ = pd;
    *q_ = *q;
}



void Worker::receive_package(Package&& p) {
    PackageSender::push_package(std::move(p));
}

void Worker::do_work(Time t) {
    if (t == 1) {
        receive_package(id_);
    }
    if (t%pd_ == 0) {
        send_package();
        receive_package(id_);
    }
}