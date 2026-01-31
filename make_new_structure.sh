#!/bin/sh
DIR_NAME=$1
mkdir -p $DIR_NAME
cd $DIR_NAME
touch "CMakeLists.txt" "${DIR_NAME}.hpp" "${DIR_NAME}.cpp"
