#include "pe.hpp"

namespace minitracer {

pe::pe(std::istream &stream)
    : stream(stream)
{}

pe::section pe::get_section(std::string name)
{
    return {};
}

}
