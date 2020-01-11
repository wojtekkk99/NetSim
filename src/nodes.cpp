//
// Created by mksyta on 18.12.2019.
//

#include "nodes.hpp"
#include <numeric>
#include <utility>
#include "iterator"
Time Worker::t_ = 0;

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator probability_function) {
    preferences_t m;
    preferences = m;
    rand_function = std::move(probability_function);
}

void ReceiverPreferences::add_receiver(IPackageReceiver * r) {
    preferences.insert(std::pair<IPackageReceiver*, double>(r, rand_function()));
    for(auto & el : preferences) {
        el.second = 1.0/preferences.size();
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver * r) {
    if(preferences.count(r)) {
        preferences.erase(r);
        for (auto &el: preferences) {
            el.second = 1.0 / preferences.size();
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double num = rand_function();
    IPackageReceiver *result = nullptr;
    double pdf = 0;
    for(auto & el: preferences){
        pdf += el.second;
        if(num < pdf)
            return el.first;
    }
    return result;
}

void PackageSender::send_package() {
    if(opt_){
        receiver_preferences_.choose_receiver()->receive_package(std::move(opt_.value()));
        opt_.reset();
    }
}

void Ramp::deliver_goods(Time t) {
    if((t-1)%di_ == 0)
        push_package(Package());
}

void Worker::do_work(Time t) {
    if(t_ == 0) {
        t_ = t;
        package_to_buf(q_->pop());
    }
    if(t - t_ == pd_ - 1){
        push_package(std::move(buf.value()));
        t_ = 0;
        buf.reset();
        if(!q_->empty()) {
            t_ = t;
            package_to_buf(q_->pop());
        }
    }
}
