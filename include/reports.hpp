//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_REPORTS_HPP
#define NET_SIM_REPORTS_HPP
#include "factory.hpp"
void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);
#endif //NET_SIM_REPORTS_HPP
