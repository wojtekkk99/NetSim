//
// Created by Mateusz on 13.12.2019.
//

#include "include/nodes.hpp"
#include "factory.hpp"

int main() {
    std::map<int, char> m;
    m[2] = 'a';
    m[3] = 'b';
    m[1] = 'c';
    for(const auto el: m){
        std::cout << el.first << " " << el.second << '\n';
    }
}


