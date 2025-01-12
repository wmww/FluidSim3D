#!/bin/bash
cd "$(dirname "${BASH_SOURCE[0]}")"
mkdir -p build
g++ -std=c++11 -g -O0 src/*.cpp widap_base/*.cpp -Ideps/SFML-2.6.2/include -DFONT_PATH='"'"$PWD/deps/UbuntuMono-R.ttf"'"' -Ldeps/SFML-2.6.2/lib -Wl,-rpath,"$PWD/deps/SFML-2.6.2/lib" -lsfml-window-d -lsfml-graphics-d -lsfml-system-d -o build/FluidSim3D
