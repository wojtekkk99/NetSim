//
// Created by Mateusz on 13.01.2020.
//

#include "reports.hpp"

void generate_structure_report(Factory& factory, std::ostream& os) {
    os << '\n';
    if (factory.ramp_begin() != factory.ramp_end()) {
        os << "== LOADING RAMPS ==" << '\n';
        for (auto it = factory.ramp_begin(); it != factory.ramp_end(); it++) {
            os << "LOADING RAMP #" << std::to_string(it->get_id());
            os << '\n';
            os << '\t' << "Delivery interval: " << std::to_string(it->get_delivery_interval());
            os << '\n';
            os << '\t' << "Receivers:";
            os << '\n';
            for (const auto i: it->receiver_preferences_.get_preferences()) {
                if (i.first->get_receiver_type() == ReceiverType::Worker)
                    os << '\t' << '\t' << "worker #" << std::to_string(i.first->get_id());
                else if (i.first->get_receiver_type() == ReceiverType::Storehouse)
                    os << '\t' << '\t' << "storehouse  #" << std::to_string(i.first->get_id());
            }
            os << '\n';
        }
    }
    os << '\n';
    if (factory.worker_begin() != factory.worker_end()) {
        os << "== WORKERS ==" << '\n';
        for (auto it = factory.worker_begin(); it != factory.worker_end(); it++) {
            os << "WORKER #" << std::to_string(it->get_id());
            os << '\n';
            os << '\t' << "Processing time: " << std::to_string(it->get_processing_duration());
            os << '\n';
            os << '\t' << "Queue type: ";
            if (it->get_queue()->get_queue_type() == PackageQueueType::FIFO)
                os << "FIFO";
            else if (it->get_queue()->get_queue_type() == PackageQueueType::LIFO)
                os << "LIFO";
            os << '\t' << "Receivers:";
            os << '\n';
            for (const auto i: it->receiver_preferences_.get_preferences()) {
                if (i.first->get_receiver_type() == ReceiverType::Worker)
                    os << '\t' << '\t' << "worker #" << std::to_string(i.first->get_id());
                else if (i.first->get_receiver_type() == ReceiverType::Storehouse)
                    os << '\t' << '\t' << "storehouse  #" << std::to_string(i.first->get_id());
            }
            os << '\n';
        }
    }
    os << '\n';
    if (factory.storehouse_begin() != factory.storehouse_end()) {
        os << "== STOREHOUSES ==" << '\n';
        for (auto it = factory.storehouse_begin(); it != factory.storehouse_end(); it++) {
            os << "STOREHOUSE #" << std::to_string(it->get_id());
            os << '\n';
        }
    }
}