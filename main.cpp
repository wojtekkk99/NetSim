//
// Created by Mateusz on 13.12.2019.
//
<<<<<<< HEAD
#include "include/package.hpp"
#include "include/storage_types.hpp"
int main() {
//    Package paczka1;
//    Package paczka2;
//    Package paczka3;
//    std::cout << paczka1.get_id() << ' ' << paczka2.get_id() << ' ' << paczka3.get_id();

    PackageQueue g(PackageQueueType::LIFO);
    g.push(Package());
    g.push(Package());
    g.push(Package());
    if (g.get_queue_type() == PackageQueueType::LIFO)
        std::cout << "LIFO";
    else
        std::cout << "FIFO";
=======

#include "include/nodes.hpp"

int main() {
    return 0;
>>>>>>> Mateusz
}

