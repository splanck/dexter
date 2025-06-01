#!/bin/bash
export PREFIX="$HOME/opt/cross64"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"
mkdir -p ./build ./bin
make all
