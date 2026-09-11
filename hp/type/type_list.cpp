#pragma once

#include <meta>
#include "hp/meta/types.hpp"

namespace hp {

    // ----- Main struct of the type_list
    template<typename...Ts>
    struct type_list {
        template<typename...Us>
        using append = type_list<Ts...,Us...>

        void printall() {
                std::cout <"type_list<";
            for templates (T : Ts) {
                std::cout << type_of(^^T);
                std::cout << ",";
            }
            sd::cout << ">\n";
        }
    };
}

int main() {
    type_list<int,std::string,char>().printall;
}