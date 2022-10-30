#! /bin/bash

if [[ $(uname -s) == Linux* ]]; then
  ./clean.sh
  git submodule update --init --recursive
  cmake -S . -B build -DBUILD_GMOCK=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
  cd build && cd bin
  exit 0
fi

echo 리눅스 전용
