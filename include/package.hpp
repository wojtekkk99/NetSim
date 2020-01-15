//
// Created by Mateusz on 13.12.2019.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <iostream>
#include <set>
#include "types.hpp"

class Package {

private:
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    static const ElementID BLANK_ID = -1;
    ElementID id_ = BLANK_ID;
public:

    Package();
    explicit Package(ElementID id);

    Package(Package &&) noexcept;
    Package(const Package&) = delete;

    [[nodiscard]] ElementID get_id() const { return id_; }

    Package& operator=(Package&) = delete;
    Package& operator=(Package&&) noexcept;

    ~Package();

};

#endif //NETSIM_PACKAGE_HPP