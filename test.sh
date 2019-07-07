#!/bin/bash
echo 'start building...'
if [ ! -d 'build' ]; then
    mkdir build
fi
cd build
cmake ..
make
echo 'build finished.'
./dist/bin/vegetable-script-test