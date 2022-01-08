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
        preferences_[r] = pg_() + 1;     // nie wiem czemy podkreśla te działania matematyczne z pg_ //NAPRAWOONE
        for (auto [receiver, p] : preferences_) {
            p = p/preferences_[r];
        }
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    /** Funkcja usuwająca odbiorcę z kontenera preferences_t_ i zmieniająca wrtości dystrybuanty
     * dla każdego pozostałego odbiorcy tak aby suma wynosiła 1 **/

    if (preferences_.size() != 0 and preferences_.size() != 1) {

        // i teraz trzeba coś zrobić z tą wartością prawdopodobieństwa odbiory usuwanego, bo suma musi wyjść =1, jakoś ją rozłożyć, dlatego wymyśliłam  po prostu, żeby wartość prawdopodobienstwa dodać  do pierwszego odbiorcy
        auto element = preferences_.begin();
        element->second += preferences_[r];
        preferences_.erase(r);
    }
    if (preferences_.size() == 1) {
        preferences_.erase(r);
    }


}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    // Funkcja losująca wartość prawdopodobieństwa a następnie szukająca odbiorcy

    ProbabilityGenerator dist = pg_;
    for (auto[receiver, p] : preferences_) {
        if (p >= dist())     // znowu podreśla działania.. //nAPRAWIONE
            return receiver;

    }
    return nullptr;
}
std::optional<Package> &PackageSender::get_sending_buffer() {
    /** Funkcja zwracająca zawartość buforu kiedy ten nie jest pusty, w przeciwnym przypadku zwraca nullptr **/
    return buffer_;
}