//
// Created by Mateusz on 13.12.2019.
//

#include "package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

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