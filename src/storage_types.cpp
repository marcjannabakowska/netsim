//
// Created by ZoJa on 14.12.2021.
//

#include "storage_types.hpp"

bool PackageQueue::empty() {
    if (s_.size() != 0)
    {
        return false;
    }
    else {
        return true;
    }
}

Package PackageQueue::pop()
{
    // FIFO


    if (q_type == PackageQueueType::FIFO)  //tutaj nie tą zmienną sprawdzałyśmy aa
    {
        auto p = std::move(s_.front());
        s_.pop_front();
        return p;
    }

    // LIFO
    else
    {
        Package p = std::move(s_.back());
        s_.pop_back();
        return p;
    }
}

