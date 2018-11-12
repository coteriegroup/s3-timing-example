#!/usr/bin/env bash
BUILD_DIR=build
if [[ -d ${BUILD_DIR} ]]; then
	rm ${BUILD_DIR} -fR
fi
exit 0
