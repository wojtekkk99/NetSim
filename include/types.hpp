//
// Created by Mateusz on 13.12.2019.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = unsigned int;
using ProbabilityGenerator = std::function<double()>;
using Time = int;
using TimeOffset = int;

#endif //NETSIM_TYPES_HPP
