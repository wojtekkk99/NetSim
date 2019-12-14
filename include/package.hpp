//
// Created by Mateusz on 13.12.2019.
//

#ifndef NETSIM_STORAGE_HPP
#define NETSIM_STORAGE_HPP

#include <iostream>
#include <set>
#include "types.hpp"

class Package {

private:
    ElementID id_;

public:
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    Package();

    Package(Package &&) = default;

    ElementID get_id() const { return id_; }

    Package &operator=(Package &&) = default;

};

#endif //NETSIM_STORAGE_HPP
