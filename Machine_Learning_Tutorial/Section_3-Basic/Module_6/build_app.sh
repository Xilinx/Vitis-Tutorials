#!/bin/bash

git pull
rm -rf build
mkdir build&&cd build
cmake ..&& make
cp *drm ../