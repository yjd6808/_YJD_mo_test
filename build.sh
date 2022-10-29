#! /bin/bash

if [[ $(uname -s) == Linux* ]]; then
  ./clean.sh
  git submodule update --init --recursive
  cmake -S . -B build -DBUILD_GMOCK=OFF
  cd build && make && cd ..
  exit 0
fi

echo 리눅스 전용
