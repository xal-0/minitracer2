#include <iostream>

#include "arch/arch.hpp"
#include "minitracer.hpp"

void h()
{
    minitracer::tracer::print();
}

void g()
{
    h();
}

void f()
{
    g();
}

int main(int argc, char **argv)
{
    minitracer::tracer::init(argv[0]);
    f();
}
