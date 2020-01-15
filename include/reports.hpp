//
// Created by Mateusz on 13.01.2020.
//

#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "nodes.hpp"

class IntervalReportNotifier {
private:
    TimeOffset to_;
public:
    explicit IntervalReportNotifier(TimeOffset to) : to_(to) {}
    bool  should_generate_report(Time t) { return (t - 1) % to_ == 0; }
};

class SpecificTurnsReportNotifier {
private:
    std::set<Time> turns_;
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {}
    bool should_generate_report(Time t) { return turns_.find(t) != turns_.end(); }
};

#endif //NETSIM_REPORTS_HPP
