//
// Created by Mateusz on 13.12.2019.
//

#include "package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

void insert_if_not_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) == s.end()) {
        s.insert(e);
    }
}

void erase_if_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) != s.end()) {
        s.erase(e);
    }
}

Package::Package() {
    if (!Package::freed_IDs.empty()) {
        ElementID minim = *Package::freed_IDs.begin();
        for (const auto el: Package::freed_IDs) {
            if (el < minim)
                minim = el;
        }
        id_ = minim;
        Package::assigned_IDs.insert(minim);
    }
    else {
        ElementID maxim = *Package::assigned_IDs.begin();
        for (const auto el: Package::assigned_IDs) {
            if (el > maxim)
                maxim = el;
        }
        id_ = maxim + 1;
        Package::assigned_IDs.insert(maxim + 1);
    }
}

Package::Package(ElementID id) {
    if (assigned_IDs.find(id) != assigned_IDs.end()) {
        throw std::invalid_argument("The ID of " + std::to_string(id) + " is already assigned!");
    }
    id_ = id;
    assigned_IDs.insert(id);
    erase_if_exists(freed_IDs, id);
}

Package::Package(Package&& other) noexcept {
    id_ = other.id_;
    other.id_ = BLANK_ID;
}

Package& Package::operator=(Package&& other) noexcept {
    if (id_ != BLANK_ID) {
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }

    id_ = other.id_;
    other.id_ = BLANK_ID;

    return *this;
}

Package::~Package() {
    if (id_ != BLANK_ID) {
        insert_if_not_exists(freed_IDs, id_);
        erase_if_exists(assigned_IDs, id_);
    }
}