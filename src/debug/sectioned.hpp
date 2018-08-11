#pragma once

#include <iostream>

namespace minitracer {

class sectioned_binary {
public:
    struct section {
        size_t offset;
        size_t size;
    };

    virtual section get_section(std::string name) = 0;
};

};
