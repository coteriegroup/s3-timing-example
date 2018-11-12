#!/usr/bin/env bash
BUILD_DIR=build
if [[ ! -d ${BUILD_DIR} ]]; then
	mkdir ${BUILD_DIR}
fi
cd ${BUILD_DIR}
cmake ..
make all
cd ..
exit 0
