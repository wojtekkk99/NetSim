//
// Created by mksyta on 18.12.2019.
//

#include "nodes.hpp"

ReceiverPreferences::ReceiverPreferences(const std::function<double()>& f) {
    preferences_t m;
    preferences = m;
    rand_function = f;
}

void ReceiverPreferences::add_receiver(IPackageReceiver * r) {
    preferences.insert(std::pair<IPackageReceiver*, double>(r, rand_function()));
    for(auto el: preferences){
        el.second = 1.0/preferences.size();
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver * r) {
    preferences.erase(r);
    for(auto el: preferences){
        el.second = 1.0/preferences.size();
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    std::uniform_real_distribution<double> num(0.0, 1.0);

}