//
// Created by ZoJa on 14.12.2021.
//

#include "storage_types.hpp"

bool IPackageStockpile::empty() {
    if (s_.size() != 0)
    {
        return false;
    }
    else {
        return true;
    }
}

PackageQueueType IPackageQueue::get_queue_type()
{
    return this->queue_type;
}

PackageQueue::PackageQueue(PackageQueueType type)
{
    this->queue_type = type;
}


Package PackageQueue::pop()
{
    // FIFO
    if(queue_type == PackageQueueType::FIFO)
    {
        Package p = s_.front();
        s_.pop_front();
        return p;
    }

    // LIFO
    if (queue_type == PackageQueueType::LIFO)
    {
        Package p = s_.back();
        s_.pop_back();
        return p;
    }


}