#!/bin/bash

(
    cd .. &&
    cmake -G Xcode -B "build" .
)
