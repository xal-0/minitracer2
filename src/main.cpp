#include <iostream>
#include <fstream>
#include <cstdlib>

#include "debug/dwarf.hpp"
#include "debug/byteio.hpp"

using namespace std;
using namespace minitracer;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cerr << "wrong number of arguments" << endl;
        exit(1);
    }

    ifstream stream {argv[1], ios_base::in | ios_base::binary};
    dwarf d {stream};
    d.read_linenums();
}
