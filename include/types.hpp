//
// Created by ZoJa on 14.12.2021.
//

#ifndef NET_SIM_TYPES_HPP
#define NET_SIM_TYPES_HPP

#include <random>
#include <ctime>
#include <functional>

//std::random_device rd;
//std::mt19937 rng(rd());

//double probability_generator_1() {
//    return std::generate_canonical<double, 10>(rng);
//}

using ElementID = unsigned int;
using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double> ;

#endif //NET_SIM_TYPES_HPP
