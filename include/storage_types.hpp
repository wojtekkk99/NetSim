//
// Created by Mateusz on 13.12.2019.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <deque>
#include "package.hpp"
#include <list>

class IPackageStockpile {
public:
    using Queue = std::deque<Package>::const_iterator;
    virtual void push(Package && package) = 0;
    [[nodiscard]] virtual bool empty() const = 0;
    [[nodiscard]] virtual Queue::size_type size() const = 0;
    virtual ~IPackageStockpile() = default;
//    [[nodiscard]] virtual Queue cbegin() const = 0;
//    [[nodiscard]] virtual Queue cend() const = 0;
//    virtual Queue begin()  = 0;
//    virtual Queue end()  = 0;
};

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageQueue : public IPackageStockpile {
    virtual Package pop() = 0;
    [[nodiscard]] virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue {
private:
    using Queue = std::list<Package>;
    Queue queue_list;
    PackageQueueType type_;
public:
    explicit PackageQueue(PackageQueueType type) : queue_list(0), type_(type) {}
    void push(Package&& package) override { queue_list.emplace_back(std::move(package)); }
    [[nodiscard]] bool empty() const override { return queue_list.empty(); }
    Package pop() override;
    [[nodiscard]] PackageQueueType get_queue_type() const override;
    [[nodiscard]] Queue::size_type size() const override { return queue_list.size(); }
    //[[nodiscard]] Queue::const_iterator cbegin() const override { return queue_list.cbegin(); }

};

#endif //NETSIM_STORAGE_TYPES_HPP
