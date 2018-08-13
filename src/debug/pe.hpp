#pragma once

#include <iostream>

#include "sectioned.hpp"

namespace minitracer {

class pe : public sectioned_binary {
public:
    pe(std::istream &stream);
    ~pe() = default;

    section get_section(std::string name) override;

private:
    std::istream &stream;
};

}
