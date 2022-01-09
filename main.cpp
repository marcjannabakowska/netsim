#include <iostream>
#include "storage_types.hpp"
#include "package.hpp"
#include "nodes.hpp"

int main() {
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    std::cout<<buffer.value().get_id();



    return 0;
}
