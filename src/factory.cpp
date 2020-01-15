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

ParsedLineData parse_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line);
    char delimiter = ' ';
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    std::string type = tokens.front();
    tokens.erase(tokens.begin());
    ParsedLineData parse = ParsedLineData();
    if(type == "LOADING_RAMP")
        parse.element_type = ElementType::LOADING_RAMP;
    else if(type == "WORKER")
        parse.element_type = ElementType::WORKER;
    else if(type == "STOREHOUSE")
        parse.element_type = ElementType::STORHOUSE;
    else if(type == "LINK")
        parse.element_type = ElementType::LINK;

    for(const auto& el: tokens) {
        std::vector<std::string> key_value;
        std::string tok;
        std::istringstream single_token(el);
        char del = '=';
        while (std::getline(single_token, tok, del)) {
            key_value.push_back(tok);
        }
        parse.parameters[key_value[0]] = key_value[1];
    }
    return parse;
}

Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;
    while (std::getline(is, line)) {
        if(line[0] == ';' or line.empty())
            continue;
        ParsedLineData parsed_line = parse_line(line);
        if(parsed_line.element_type == ElementType::LOADING_RAMP) {
            ElementID id = 0;
            TimeOffset di = 0;
            for(const auto&el: parsed_line.parameters){
                if(el.first == "id")
                    id = std::stoi(el.second);
                else if(el.first == "delivery-interval")
                    di = std::stoi(el.second);
            }
            factory.add_ramp(Ramp(id, di));
        }
        else if(parsed_line.element_type == ElementType::WORKER) {
            PackageQueueType q;
            ElementID id = 0;
            TimeOffset pd = 0;
            for (const auto &el: parsed_line.parameters) {
                if (el.first == "id")
                    id = std::stoi(el.second);
                else if (el.first == "processing-time")
                    pd = std::stoi(el.second);
                else if (el.first == "queue-type") {
                    if (el.second == "FIFO")
                        q = PackageQueueType::FIFO;
                    else if (el.second == "LIFO")
                        q = PackageQueueType::LIFO;
                }
            }
            std::unique_ptr<IPackageQueue> qq = std::make_unique<PackageQueue>(q);
            factory.add_worker(Worker(id, pd, std::move(qq)));
        }
        else if(parsed_line.element_type == ElementType::STORHOUSE)
            factory.add_storehouse(Storehouse(std::stoi(parsed_line.parameters["id"])));
        else if(parsed_line.element_type == ElementType::LINK){
            std::string src;
            std::string src_id;
            std::string dest;
            std::string dest_id;
            for(const auto& el: parsed_line.parameters) {
                if (el.first == "src") {
                    std::vector<std::string> key_value;
                    std::string tok;
                    std::istringstream single_token(el.second);
                    char del = '-';
                    while (std::getline(single_token, tok, del))
                        key_value.push_back(tok);
                    src = key_value[0];
                    src_id = key_value[1];
                } else if (el.first == "dest") {
                    std::vector<std::string> key_value;
                    std::string tok;
                    std::istringstream single_token(el.second);
                    char del = '-';
                    while (std::getline(single_token, tok, del))
                        key_value.push_back(tok);
                    dest = key_value[0];
                    dest_id = key_value[1];
                }
            }
            if(src == "worker"){
                if(dest == "store") {
                    auto w = factory.find_worker_by_id(std::stoi(src_id));
                    auto s = factory.find_storehouse_by_id(std::stoi(dest_id));
                    w->receiver_preferences_.add_receiver(&(*s));
                }
                else if(dest == "worker"){
                    auto w = factory.find_worker_by_id(std::stoi(src_id));
                    auto w2 = factory.find_worker_by_id(std::stoi(dest_id));
                    w->receiver_preferences_.add_receiver(&(*w2));
                }
            }
            else if(src == "ramp"){
                if(dest == "worker") {
                    auto r = factory.find_ramp_by_id(std::stoi(src_id));
                    auto w = factory.find_worker_by_id(std::stoi(dest_id));
                    r->receiver_preferences_.add_receiver(&(*w));
                }
                else if(dest == "store") {
                    auto r = factory.find_ramp_by_id(std::stoi(src_id));
                    auto s = factory.find_storehouse_by_id(std::stoi(dest_id));
                    r->receiver_preferences_.add_receiver(&(*s));
                }
            }
        }
    }
    return factory;
}

void save_factory_structure(Factory& factory, std::ostream& os) {
    if (factory.ramp_begin() != factory.ramp_end()) {
        os << "; == LOADING_RAMPS ==" << '\n' << '\n';
        for (auto it = factory.ramp_begin(); it != factory.ramp_end(); it++) {
            os << "LOADING_RAMP " << "id=" << std::to_string(it->get_id()) << " delivery-interval="
               << std::to_string(it->get_delivery_interval());
            os << '\n';
        }
    }
    os << '\n';
    if (factory.worker_begin() != factory.worker_end()) {
        os << "; == WORKERS ==" << '\n' << '\n';
        for (auto it = factory.worker_begin(); it != factory.worker_end(); it++) {
            os << "WORKER " << "id=" << std::to_string(it->get_id()) << " processing-time="
               << std::to_string(it->get_processing_duration()) << " queue-type=";
            if (it->get_queue()->get_queue_type() == PackageQueueType::FIFO)
                os << "FIFO";
            else if (it->get_queue()->get_queue_type() == PackageQueueType::LIFO)
                os << "LIFO";
            os << '\n';
        }
    }
    os << '\n';
    if (factory.storehouse_begin() != factory.storehouse_end()) {
        os << "; == STOREHOUSES ==" << '\n' << '\n';
        for (auto it = factory.storehouse_begin(); it != factory.storehouse_end(); it++) {
            os << "STOREHOUSE " << "id=" << std::to_string(it->get_id());
            os << '\n';
        }
    }
    os << '\n';
    os << "; == LINKS ==" << '\n';
    os << '\n';
    if (factory.ramp_begin() != factory.ramp_end()) {
        for (auto it = factory.ramp_begin(); it != factory.ramp_end(); it++) {
            for(auto el = it->receiver_preferences_.get_preferences().rbegin(); el != it->receiver_preferences_.get_preferences().rend(); el++){
                os << "LINK " << "src=ramp-" << it->get_id() << " dest=";
                if(el->first->get_receiver_type() == ReceiverType::Worker)
                    os << "worker-" << el->first->get_id();
                else if(el->first->get_receiver_type() == ReceiverType::Storehouse)
                    os << "store-" << el->first->get_id();
                os << "\n";
            }
           os << "\n";
        }
    }
    if (factory.worker_begin() != factory.worker_end()) {
        for (auto it = factory.worker_begin(); it != factory.worker_end(); it++) {
            for(auto el = it->receiver_preferences_.get_preferences().rbegin(); el != it->receiver_preferences_.get_preferences().rend(); el++){
                os << "LINK " << "src=worker-" << it->get_id() << " dest=";
                if(el->first->get_receiver_type() == ReceiverType::Worker)
                    os << "worker-" << el->first->get_id();
                else if(el->first->get_receiver_type() == ReceiverType::Storehouse)
                    os << "store-" << el->first->get_id();
                os << "\n";
            }
            os << '\n';
        }
    }
}