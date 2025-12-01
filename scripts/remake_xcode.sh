#!/bin/bash

source ./build_dir.sh

(
    cd .. &&
    rm -rf "$BUILD_DIR"
)

source ./make_xcode.sh
