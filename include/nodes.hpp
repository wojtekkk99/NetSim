//
// Created by mksyta on 18.12.2019.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "storage_types.hpp"
#include <memory>
enum class ReceiverType {
    Ramp, Worker, Storehouse
};

class IPackageReceiver {
public:
    [[nodiscard]] virtual IPackageStockpile::QueueTCI cbegin() const = 0;
    [[nodiscard]] virtual IPackageStockpile::QueueTCI cend() const = 0;
    virtual IPackageStockpile::QueueTCI begin()  = 0;
    virtual IPackageStockpile::QueueTCI end()  = 0;
    virtual void receive_package(Package&& p) = 0;
    virtual ~IPackageReceiver() = default;
    [[nodiscard]] virtual ReceiverType get_receiver_type() const = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
};

class Storehouse: public IPackageReceiver {
private:
     std::unique_ptr<IPackageStockpile> d_;
     ElementID id_;
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : d_(std::move(d)), id_(id) {}
    [[nodiscard]] IPackageStockpile::QueueTCI cbegin() const override { return d_ -> cbegin(); }
    IPackageStockpile::QueueTCI begin()  override { return d_ -> begin(); }
    [[nodiscard]] IPackageStockpile::QueueTCI cend() const override { return d_ -> cend(); }
    IPackageStockpile::QueueTCI end()  override { return d_ -> end(); }
    [[nodiscard]] ReceiverType get_receiver_type() const override  { return ReceiverType::Storehouse; }
    [[nodiscard]] ElementID get_id() const override { return id_; }
    void receive_package(Package&& p) override { d_ -> push(std::move(p)); }


};

#endif //NETSIM_NODES_HPP
