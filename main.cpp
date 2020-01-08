//
// Created by Mateusz on 13.12.2019.
//

#include "include/nodes.hpp"

double numm() {
    return 0.5;
}

int main() {
    std::function<double()> t = numm;
    ReceiverPreferences rt(numm);
    PackageQueue g(PackageQueueType::FIFO);
    std::unique_ptr<IPackageQueue> q = std::make_unique<PackageQueue>(std::move(g));

    Worker w1(1,1,std::move(q));
    Worker w2(2,1,std::move(q));
    Worker w3(3,1,std::move(q));
    Worker* wg1 = &w1;
    Worker* wg2 = &w2;
    Worker* wg3 = &w3;
    rt.add_receiver(wg1);
    rt.add_receiver(wg2);
    rt.add_receiver(wg3);
    auto gf = rt.choose_receiver();
    std::cout << gf->get_id();
    return 0;
}

