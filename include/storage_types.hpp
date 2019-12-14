//
// Created by Mateusz on 13.12.2019.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <deque>
#include "package.hpp"

class IPackageStockpile {
public:
    using Queue = std::deque<Package>::const_iterator;

    virtual  void push(const Package &&) = 0;

    [[nodiscard]] virtual bool empty() const = 0;

    [[nodiscard]] virtual Queue::size_type size() const = 0;
};

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageQueue {
    virtual Package pop() = 0;

    [[nodiscard]] virtual PackageQueueType get_queue_type() const = 0;
};

#endif //NETSIM_STORAGE_TYPES_HPP
