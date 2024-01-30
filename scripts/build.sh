#!/bin/bash

# Function to check if an argument is provided
check_arguments() {
    if [ $# -eq 0 ]; then
        echo "write a valid packet to compile"
        exit 1
    fi
}

# Function to check if the packet is valid
check_valid_packet() {
    if [ ! -d "$type/$packet" ]; then
        ls
        echo "No valid packet $type/$packet"
        exit 1
    fi
}

# Function to compile the packet
compile_packet() {
    echo "Compiling $packet $type"

    cd $type/$packet
    
    if [ ! -d "build/" ]; then
        mkdir build
    fi
    
    cd build
    export CC=~/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc
    export CXX=~/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++
    export LDFLAGS="--specs=nosys.specs"
    cmake .. > build.log 2>&1
    make >> build.log 2>&1
}

# Function to compile the library
compile_library() {
    if [ ! -d "build/" ]; then
       mkdir build
    fi
    
    cd build
    # export CC=~/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc
    # export CXX=~/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++
    cmake .. > build.log 2>&1
    make >> build.log
}

# Navigate to the parent directory
cd ..

# Check for arguments
check_arguments "$@"

type=$1
packet=$2

# Check if type is "library" and compile accordingly
if [ "$type" = "library" ]; then
    compile_library
else
    # Check if the packet is valid
    check_valid_packet
    # Compile the packet
    compile_packet
fi
