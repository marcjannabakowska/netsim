//
// Created by ZoJa on 14.12.2021.
//

#include "storage_types.hpp"

bool IPackageQueue::empty() {
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
    if (queue_type == PackageQueueType::FIFO)
    {
        auto p = std::move(s_.front());
        s_.pop_front();
        return p;
    }

    // LIFO
    if (queue_type == PackageQueueType::LIFO)
    {
        Package p = std::move(s_.back());
        s_.pop_back();
        return p;
    }


}