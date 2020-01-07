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
    virtual IPackageStockpile::iterator begin()  = 0;
    virtual IPackageStockpile::iterator end()  = 0;
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
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return d_ -> cbegin(); }
    IPackageStockpile::iterator begin()  override { return d_ -> begin(); }
    [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return d_ -> cend(); }
    IPackageStockpile::iterator end()  override { return d_ -> end(); }
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
    ProbabilityGenerator rand_function;
    preferences_t preferences;
public:
    [[nodiscard]] preferences_t& get_preferences() const { return preferences; }
    explicit ReceiverPreferences(const ProbabilityGenerator& f);
    ReceiverPreferences() = delete;
    [[nodiscard]] const_iterator cbegin() const { return preferences.cbegin(); }
    [[nodiscard]] const_iterator cend() const { return preferences.cend(); }
    iterator begin() { return preferences.begin(); }
    iterator end() { return preferences.end(); }
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
    ReceiverPreferences receiver_preferences_;
    explicit PackageSender(ReceiverPreferences receiver_preferences) : receiver_preferences_(std::move(receiver_preferences)) {}
    PackageSender() : receiver_preferences_(pg_help) {}
    void send_package();
    [[nodiscard]] std::optional<Package>& get_sending_buffer() const { return opt_; }
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
    std::optional<Package> buf;
public:
    static Time t_;
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : q_(std::move(q)), id_(id), pd_(pd), buf(std::nullopt) {};
    void do_work(Time t);
    void package_to_buf(Package&& p) { buf = std::move(p); }
    [[nodiscard]] TimeOffset get_processing_duration() const { return pd_; }
    [[nodiscard]] static Time get_package_processing_start_time() { return t_; }
    [[nodiscard]]  IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    [[nodiscard]]  IPackageStockpile::const_iterator cend() const override { return q_->cend(); }
    IPackageStockpile::iterator begin()  override { return q_->begin(); }
    IPackageStockpile::iterator end()  override { return q_->end(); }
    [[nodiscard]] ReceiverType get_receiver_type() const override { return ReceiverType::Worker; }
    void receive_package(Package&& p) override  {q_ -> push(std::move(p)); }
    [[nodiscard]] ElementID get_id() const override { return id_; }
};

#endif //NETSIM_NODES_HPP
