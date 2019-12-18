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
    using QueueTCI = QueueT::const_iterator;
    virtual void push(Package && package) = 0;
    [[nodiscard]] virtual bool empty() const = 0;
    [[nodiscard]] virtual QueueT::size_type size() const = 0;
    virtual ~IPackageStockpile() = default;
    [[nodiscard]] virtual QueueTCI cbegin() const = 0;
    [[nodiscard]] virtual QueueTCI cend() const = 0;
    virtual QueueTCI begin()  = 0;
    virtual QueueTCI end()  = 0;
};

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageQueue : public IPackageStockpile {
    virtual Package pop() = 0;

    [[nodiscard]] virtual PackageQueueType get_queue_type() const = 0;

protected:
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
    [[nodiscard]] QueueTCI cbegin() const override { return queue_list.cbegin(); }
    [[nodiscard]] QueueTCI begin()  override { return queue_list.begin(); }
    [[nodiscard]] QueueTCI cend() const override { return queue_list.cend(); }
    [[nodiscard]] QueueTCI end()  override { return queue_list.end(); }

};

#endif //NETSIM_STORAGE_TYPES_HPP
