//
// Created by Mateusz on 06.01.2020.
//

#include "factory_package.hpp"

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
