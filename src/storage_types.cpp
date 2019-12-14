//
// Created by Mateusz on 13.12.2019.
//

#include "storage_types.hpp"

Package PackageQueue::pop() {
    if (get_queue_type() == PackageQueueType::FIFO) {
        Package package = std::move(queue_list.front());
        queue_list.pop_front();
        return package;
    } else {
        Package package = std::move(queue_list.back());
        queue_list.pop_back();
        return package;
    }
}

PackageQueueType PackageQueue::get_queue_type() const {
    switch(type_) {
        case PackageQueueType::FIFO:
            return PackageQueueType::FIFO;
        case PackageQueueType::LIFO:
            return PackageQueueType::LIFO;
    }
}
