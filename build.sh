#!/bin/bash
cd "$(dirname "${BASH_SOURCE[0]}")"

FONT_PATH="$PWD/deps/UbuntuMono-R.ttf"
SFML_DIR="$PWD/deps/SFML-2.6.2"
SFML_SUFFIX=""
CXX_FLAGS="-std=c++11"

case "$1" in
  -r)
    CXX_FLAGS="$CXX_FLAGS -O2"
    ;;

  -d)
    CXX_FLAGS="$CXX_FLAGS -O0 -g"
    SFML_SUFFIX="-d"
    ;;

  *)
    echo "Pass -r for release build or -d for debug build"
    exit 1
    ;;
esac

mkdir -p build
g++ \
  $CXX_FLAGS \
  src/*.cpp widap_base/*.cpp \
  -DFONT_PATH='"'"$FONT_PATH"'"' \
  -I"$SFML_DIR/include" -L"$SFML_DIR/lib" -Wl,-rpath,"$SFML_DIR/lib" \
  -l"sfml-window$SFML_SUFFIX" -l"sfml-graphics$SFML_SUFFIX" -l"sfml-system$SFML_SUFFIX" \
  -o build/FluidSim3D
