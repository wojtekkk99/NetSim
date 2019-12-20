//
// Created by mksyta on 18.12.2019.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "storage_types.hpp"
#include <memory>
#include <map>
#include <functional>
#include <random>

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

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
private:
    preferences_t preferences;
    std::function<double()> rand_function;
public:
    explicit ReceiverPreferences(const std::function<double()>& f);
    [[nodiscard]] const_iterator cbegin() const { return preferences.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return preferences.cend(); }
    iterator begin() { return preferences.begin(); }
    iterator end() { return preferences.end(); }
    IPackageReceiver* choose_receiver();
    void remove_receiver(IPackageReceiver* r);
    void add_receiver(IPackageReceiver* r);

};

#endif //NETSIM_NODES_HPP
