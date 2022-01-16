//
// Created by ZoJa on 14.12.2021.
//

#include "simulation.hpp"
//void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf){
//    if (!f.is_consistent()) { throw std::logic_error("non consistent network!"); }
//    for(TimeOffset i = 0; i<d; i++){
//        f.do_deliveries(i);
//        f.do_package_passing();
//        f.do_work(i);
//        rf(f,i);
//    }
//}