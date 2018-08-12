#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

#include "debug/elf.hpp"
#include "debug/dwarf.hpp"
#include "debug/byteio.hpp"
#include "debug/sectioned.hpp"

using namespace std;
using namespace minitracer;

int main(int argc, char **argv)
{
    if (argc != 3) {
        cerr << "wrong number of arguments" << endl;
        exit(1);
    }

    ifstream stream {argv[1], ios_base::in | ios_base::binary};
    elf binary {stream};
    dwarf d {stream, binary};

    stringstream ss;
    ss << hex << argv[2];

    uaddr addr;
    ss >> addr;

    auto m = d.get_linenum(addr);

    if (m.file->directory)
        cout << hex << m.address << "\t\t"
             << *m.file->directory << "/"
             << m.file->filename << ":"
             << dec << m.line << "\n";
    else
        cout << hex << m.address << "\t\t"
             << m.file->filename << ":"
             << dec << m.line << "\n";
}
