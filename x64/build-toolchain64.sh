#!/bin/bash
export PREFIX="$HOME/opt/cross64"
export TARGET=x86_64-elf  # Changed from i686-elf to x86_64-elf
export PATH="$PREFIX/bin:$PATH"
export BINVER="2.41"
export GCCVER="12.2.0"

mkdir -p $HOME/source/x64
cd $HOME/source/x64

wget https://ftp.gnu.org/gnu/binutils/binutils-$BINVER.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-$GCCVER/gcc-$GCCVER.tar.gz

tar -xvzf binutils-$BINVER.tar.gz
tar -xvzf gcc-$GCCVER.tar.gz

mkdir build-binutils
cd build-binutils

../binutils-$BINVER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd $HOME/source

mkdir build-gcc
cd build-gcc

../gcc-$GCCVER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
