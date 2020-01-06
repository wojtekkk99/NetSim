//
// Created by Mateusz on 06.01.2020.
//

#ifndef NETSIM_FACTORY_PACKAGE_HPP
#define NETSIM_FACTORY_PACKAGE_HPP

#include "types.hpp"
#include "nodes.hpp"
#include <algorithm>

template <class Node>
class NodeCollection {
private:
    std::list<Node> node_list;
public:
    using iterator = typename std::list<Node>::iterator;
    using const_iterator = typename std::list<Node>::const_iterator;
    iterator begin() { return node_list.begin(); }
    [[nodiscard]] const_iterator cbegin() const { return node_list.cbegin(); }
    iterator end() { return node_list.end(); }
    [[nodiscard]] const_iterator cend() const { return node_list.cend(); }
    void add(Node& node) { node_list.emplace(std::move(node)); }
    void remove_by_id(ElementID id);
    iterator find_by_id(ElementID id_);
    [[nodiscard]] const_iterator find_by_id(ElementID id_) const;
};

class Factory {
private:
    NodeCollection<Worker> workers_list;
    NodeCollection<Ramp> ramps_list;
    NodeCollection<Storehouse> storehouses_list;
public:
    void add_ramp(Ramp&& r) { ramps_list.add(r); }
    void remove_ramp(ElementID id) { ramps_list.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_list.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramps_list.find_by_id(id); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps_list.cbegin(); }
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps_list.cend(); }

    void add_worker(Worker&& w) { workers_list.add(w); }
    void remove_worker(ElementID id) { workers_list.remove_by_id((id)); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers_list.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return workers_list.find_by_id(id); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers_list.cbegin(); }
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const { return workers_list.cend(); }

    void add_storehouse(Storehouse&& s) { storehouses_list.add(s); }
    void remove_storehouse(ElementID id) { storehouses_list.remove_by_id(id); }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { storehouses_list.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouses_list.find_by_id(id); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses_list.cbegin(); }
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses_list.cend(); }

    void remove_receiver(NodeCollection<Node> collection, ElementID id);
    [[nodiscard]] bool is_consistent() const;
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);
};


template<class Node>
typename NodeCollection<Node>::iterator NodeCollection<Node>::find_by_id(ElementID id_) {
    auto it = std::find_if(node_list.begin(), node_list.end(), [id_](const auto& elem){ return (elem->get_id() == id_);});
    return it;
}

template<class Node>
typename NodeCollection<Node>::const_iterator NodeCollection<Node>::find_by_id(ElementID id_) const {
    auto it = std::find_if(node_list.cbegin(), node_list.cend(), [id_](const auto& elem){ return (elem->get_id() == id_);});
    return it;
}

template<class Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    auto it = find_by_id(id);
    if (it != node_list.end())
        node_list.erase(it);
}


#endif //NETSIM_FACTORY_PACKAGE_HPP