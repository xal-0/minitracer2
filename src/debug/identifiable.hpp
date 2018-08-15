#pragma once

#include <iostream>

namespace minitracer {

class identifiable {
public:
    static bool identify(std::istream &stream);
};

}
