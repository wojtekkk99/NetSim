//
// Created by Mateusz on 06.01.2020.
//

#include "factory.hpp"

void Factory::do_work(Time t) {
    std::for_each(workers_list.begin(), workers_list.end(), [t](auto& elem){ elem.do_work(t); });
}

void Factory::do_deliveries(Time t) {
    std::for_each(ramps_list.begin(), ramps_list.end(), [t](auto& elem){ elem.deliver_goods(t); });
}

void Factory::do_package_passing() {
    std::for_each(ramps_list.begin(), ramps_list.end(), [](auto& elem){ elem.send_package(); });
    std::for_each(workers_list.begin(), workers_list.end(), [](auto& elem){ elem.send_package(); });
}

template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    for (auto &el: collection) {
        for(auto& v: el.receiver_preferences_.get_preferences())
            if(v.first->get_id() == id) {
                el.receiver_preferences_.remove_receiver(v.first);
                break;
            }
    }
}

void Factory::remove_ramp(ElementID id) {
    ramps_list.remove_by_id(id);
}

void Factory::remove_worker(ElementID id) {
    remove_receiver(ramps_list, id);
    remove_receiver(workers_list, id);
    workers_list.remove_by_id(id);

}

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(workers_list, id);
    storehouses_list.remove_by_id(id);
}

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if(node_colors[sender] == NodeColor::VERIFIED)
        return true;
    node_colors[sender] = NodeColor::VISITED;
    if(sender->receiver_preferences_.get_preferences().empty()) {
        throw NoReceiversException();
    }
    bool has_other_receiver = false;
    for(const auto& el: sender->receiver_preferences_.get_preferences()){
        if(el.first->get_receiver_type() == ReceiverType::Storehouse)
            has_other_receiver = true;
        else if(el.first->get_receiver_type() == ReceiverType::Worker){
            IPackageReceiver* receiver_ptr = el.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender)
                continue;
            has_other_receiver = true;
            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED)
                has_reachable_storehouse(sendrecv_ptr, node_colors);
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if(has_other_receiver)
        return true;
    else
        throw NoReceiversException();
}

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> node_colors;
    for(const auto& el: workers_list)
        node_colors[(PackageSender *) &el] = NodeColor::UNVISITED;
    for(const auto& el: ramps_list)
        node_colors[(PackageSender* ) &el] = NodeColor::UNVISITED;
    for(const auto& el: ramps_list)
    {
        try {
            has_reachable_storehouse(&el, node_colors);
            return true;
        }
        catch(NoReceiversException& noReceiversException){
            return false;
        }
    }
    return true;
}


