//
// Created by Mateusz on 13.12.2019.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <deque>
#include "package.hpp"

class IPackageStockpile {
public:
    using Queue = std::deque<Package>::const_iterator;

    virtual  void push(const Package &&) = 0;

    virtual bool empty() const = 0;

    virtual Queue::size_type size() const = 0;
};

#endif //NETSIM_STORAGE_TYPES_HPP
