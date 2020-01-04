//
// Created by mksyta on 18.12.2019.
//

#include "nodes.hpp"

Time Worker::t_ = 0;

ReceiverPreferences::ReceiverPreferences(const ProbabilityGenerator& f) {
    preferences_t m;
    preferences = m;
    rand_function = f;
}

void ReceiverPreferences::add_receiver(IPackageReceiver * r) {
    preferences.insert(std::pair<IPackageReceiver*, double>(r, rand_function()));
    for(auto & el : preferences) {
        el.second = 1.0/preferences.size();
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver * r) {
    preferences.erase(r);
    for(auto & el: preferences){
        el.second = 1.0/preferences.size();
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double num = rand_function();
    double period = 0;
    for(int ind = 1; ind <= int(preferences.size()); ind++){
        if (num >= double(ind-1)/preferences.size() && num < double(ind)/preferences.size()){
            period = double(ind)/preferences.size();
            break;
        }
    }
    IPackageReceiver *result = nullptr;
    for(const auto el: preferences){
        if(el.second == period)
            result = el.first;
    }
    return result;
}

void PackageSender::send_package() {
    receiver_preferences_.choose_receiver()->receive_package(std::move(opt_.value()));
    opt_.reset();
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
    if((t - t_) == pd_){
        push_package(std::move(buf.value()));
        t_ = 0;
        buf.reset();
    }
}
