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

    int index = 0;
    // FIFO
    if(queue_type == PackageQueueType::FIFO)
        index = 0;
    // LIFO
    if (queue_type == PackageQueueType::LIFO)
        index = s_.size() - 1;

    Package p =   // i tu nie wiem jak wyciągnąc element o indeksie index z tego s_

    return p;
}