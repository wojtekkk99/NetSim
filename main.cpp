//
// Created by Mateusz on 13.12.2019.
//

#include "include/nodes.hpp"
#include "factory.hpp"
double numm() {
    return 0.5;
}

int main() {
    std::string t = "WORKER id=3 delivery-interval=5";
    auto p = parse_line(t);
    return 0;
}

