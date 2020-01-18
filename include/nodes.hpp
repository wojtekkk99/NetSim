//
// Created by mksyta on 18.12.2019.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"
#include <memory>
#include <map>
#include <optional>
#include <utility>

enum class ReceiverType {
    Ramp, Worker, Storehouse
};

class IPackageReceiver {
public:
    [[nodiscard]] virtual IPackageStockpile::const_iterator cbegin() const = 0;
    [[nodiscard]] virtual IPackageStockpile::const_iterator cend() const = 0;
    [[nodiscard]] virtual IPackageStockpile::const_iterator begin() const = 0;
    [[nodiscard]] virtual IPackageStockpile::const_iterator end() const = 0;
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
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))) : d_(std::move(d)), id_(id) {}
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return d_ -> cbegin(); }
    [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return d_ -> begin(); }
    [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return d_ -> cend(); }
    [[nodiscard]] IPackageStockpile::const_iterator end() const override { return d_ -> end(); }
    [[nodiscard]] ReceiverType get_receiver_type() const override  { return ReceiverType::Storehouse; }
    [[nodiscard]] ElementID get_id() const override { return id_; }
    void receive_package(Package&& p) override { d_ -> push(std::move(p)); }
    [[nodiscard]]  IPackageStockpile* get_queue() const { return d_.get(); }

};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
private:
    ProbabilityGenerator rand_function;
    preferences_t preferences_;
public:
    [[nodiscard]] const preferences_t& get_preferences() const { return preferences_; }
    explicit ReceiverPreferences(ProbabilityGenerator probability_function = probability_generator);
    [[nodiscard]] const_iterator cbegin() const { return preferences_.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return preferences_.cend(); }
    [[nodiscard]] const_iterator begin() const { return preferences_.begin(); }
    [[nodiscard]] const_iterator end() const { return preferences_.end(); }
    IPackageReceiver* choose_receiver();
    void remove_receiver(IPackageReceiver* r);
    void add_receiver(IPackageReceiver* r);

};

class PackageSender {
private:
    std::optional<Package> opt_ = std::nullopt;
protected:
    void push_package(Package&& p) { opt_ = std::move(p); }
public:
    PackageSender(PackageSender&&)=default;
    ReceiverPreferences receiver_preferences_;
    explicit PackageSender(ReceiverPreferences receiver_preferences) : receiver_preferences_(std::move(receiver_preferences)) {}
    PackageSender() : receiver_preferences_(probability_generator) {}
    void send_package();
    [[nodiscard]] const std::optional<Package>& get_sending_buffer() const { return (std::optional<Package>&) opt_; }
};

class Ramp : public PackageSender {
private:
    ElementID id_;
    TimeOffset di_;
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    void deliver_goods(Time t);
    [[nodiscard]] TimeOffset get_delivery_interval() const { return di_; }
    [[nodiscard]] ElementID get_id() const { return id_; }
    [[nodiscard]] static ReceiverType get_receiver_type()  { return ReceiverType::Ramp; }
};

class Worker : public PackageSender, public IPackageReceiver {
private:
    std::unique_ptr<IPackageQueue> q_;
    ElementID id_;
    TimeOffset pd_;
    std::optional<Package> buf  = std::nullopt;
public:
    static Time t_;
    [[nodiscard]]  IPackageQueue* get_queue() const { return q_.get(); }
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : q_(std::move(q)), id_(id), pd_(pd), buf(std::nullopt) {};
    void do_work(Time t);
    void package_to_buf(Package&& p) { buf = std::move(p); }
    [[nodiscard]] TimeOffset get_processing_duration() const { return pd_; }
    [[nodiscard]] static Time get_package_processing_start_time() { return t_; }
    [[nodiscard]]  IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    [[nodiscard]]  IPackageStockpile::const_iterator cend() const override { return q_->cend(); }
    [[nodiscard]] IPackageStockpile::const_iterator begin() const override { return q_->begin(); }
    [[nodiscard]] IPackageStockpile::const_iterator end() const override { return q_->end(); }
    [[nodiscard]] ReceiverType get_receiver_type() const override { return ReceiverType::Worker; }
    void receive_package(Package&& p) override  {q_ -> push(std::move(p)); }
    [[nodiscard]] ElementID get_id() const override { return id_; }
    [[nodiscard]] std::optional<Package>& get_processing_buffer() const { return (std::optional<Package>&) buf; }
};

#endif //NETSIM_NODES_HPP
