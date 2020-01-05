//
// Created by Mateusz on 02.01.2020.
//

#ifndef NETSIM_HELPERS_HPP
#define NETSIM_HELPERS_HPP

#include <random>
#include <ctime>

inline std::random_device rd;
inline std::mt19937 rng(rd());

inline double pg_help() { return std::generate_canonical<double, 10>(rng); }

#endif //NETSIM_HELPERS_HPP
