#!/bin/sh

set -e

DIR=$(mktemp -d)
pushd $DIR
cat >test.cpp <<EOF
#include <iostream>
int main()
{
  std::cout << "hello\n";
}
EOF
g++ -v test.cpp 2> gcc-out
awk '/search starts here/{flag=1;next}/End of search/{flag=0}flag' gcc-out > includes
INCLUDES=$(sed 's/^/-I/' < includes | tr '\n' ' ')
popd
sed "s#p\",#p $INCLUDES\",#" compile_commands.json -i
