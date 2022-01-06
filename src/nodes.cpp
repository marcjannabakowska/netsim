//
// Created by ZoJa on 14.12.2021.
//
#include "nodes.hpp"

Ramp::Ramp(ElementID id, TimeOffset di) {
    id_ = id;
    di_ = di;
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) {
    id_ = id;
    pd_ = pd;
    *q_ = *q;
}

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) {
    id_ = id;
    d_ = std::move(d);
}



void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    // Funkcja dodająca odbiorcę do kontenera preferences_t_ i zmieniająca wrtości dystrybuanty


    if (preferences_.size() == 0) {
        preferences_[r] = 1;
    }
    else {
        preferences_[r] = pg_ + 1;     // nie wiem czemy podkreśla te działania matematyczne z pg_
        for (auto [receiver, p] : preferences_) {
            p = p/preferences_[r];
        }
    }
}


IPackageReceiver *ReceiverPreferences::choose_receiver() {
    // Funkcja losująca wartość prawdopodobieństwa a następnie szukająca odbiorcy

    ProbabilityGenerator dist = pg_;
    for (auto[receiver, p] : preferences_) {
        if (p >= dist)     // znowu podreśla działania..
            return receiver;
        else
            return nullptr;
    }
}
std::optional<Package> &PackageSender::get_sending_buffer() {
    /** Funkcja zwracająca zawartość buforu kiedy ten nie jest pusty, w przeciwnym przypadku zwraca nullptr **/
    return buffer_;
}