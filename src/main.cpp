#include <iostream>

#include "arch/arch.hpp"
#include "minitracer.hpp"

int main(int argc, char **argv)
{
    minitracer::tracer::init();
    minitracer::tracer::print();
}
