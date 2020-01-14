//
// Created by Mateusz on 13.01.2020.
//

#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"

void simulate(Factory& f,TimeOffset d,const std::function<void (Factory&, Time)>& rf);

#endif //NETSIM_SIMULATION_HPP
