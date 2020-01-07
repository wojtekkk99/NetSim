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
    using QueueT = std::list<Package>;
    using const_iterator = std::list<Package>::const_iterator;
    using iterator = std::list<Package>::iterator;
    virtual void push(Package && package) = 0;
    [[nodiscard]] virtual bool empty() const = 0;
    [[nodiscard]] virtual QueueT::size_type size() const = 0;
    virtual ~IPackageStockpile() = default;
    [[nodiscard]] virtual const_iterator cbegin() const = 0;
    [[nodiscard]] virtual const_iterator cend() const = 0;
    virtual iterator begin()  = 0;
    virtual iterator end()  = 0;
};

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;
    [[nodiscard]] virtual PackageQueueType get_queue_type() const = 0;
    ~IPackageQueue() override = default;
};

class PackageQueue : public IPackageQueue {
private:
    QueueT queue_list;
    PackageQueueType type_;
public:
    explicit PackageQueue(PackageQueueType type) : queue_list(0), type_(type) {}
        void push(Package&& package) override { queue_list.emplace_back(std::move(package)); }
    [[nodiscard]] bool empty() const override { return queue_list.empty(); }
    Package pop() override;
    [[nodiscard]] PackageQueueType get_queue_type() const override;
    [[nodiscard]] QueueT::size_type size() const override { return queue_list.size(); }
    [[nodiscard]] const_iterator cbegin() const override { return queue_list.cbegin(); }
    iterator begin()  override { return queue_list.begin(); }
    [[nodiscard]] const_iterator cend() const override { return queue_list.cend(); }
    iterator end()  override { return queue_list.end(); }

};

#endif //NETSIM_STORAGE_TYPES_HPP
