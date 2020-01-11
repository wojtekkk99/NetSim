//
// Created by Mateusz on 13.12.2019.
//

#include "include/nodes.hpp"

double numm() {
    return 0.5;
}

int main() {
    ReceiverPreferences rt(numm);
    PackageQueue g(PackageQueueType::FIFO);
    std::unique_ptr<IPackageQueue> q = std::make_unique<PackageQueue>(std::move(g));
    Worker w1(1,1,std::move(q));
    w1.receive_package(Package());
    Time t = 1;
    w1.do_work(t);
    ++t;
    w1.do_work(t);
    ++t;
    w1.do_work(t);

    return 0;
}

