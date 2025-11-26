#!/bin/bash

source ./build_dir.sh

(
    cd .. &&
    cmake -G Xcode -B "$BUILD_DIR" .
)
