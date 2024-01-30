#!/bin/bash

# Function to check if an argument is provided
check_arguments() {
    if [ $# -eq 0 ]; then
        echo "write a valid packet to clean"
        exit 1
    fi
}

# Function to check if the packet is valid
check_valid_packet() {
    if [ ! -d "$type/$packet" ]; then
        echo "No valid packet"
        exit 1
    fi
}

# Function to compile the packet
clean_packet() {

    echo "Cleaning $packet $type"
    cd $type/$packet
    
    if [ -d "build/" ] 
    then
        rm -r build
    else 
        echo No build directory found
    fi    

}

# Function to compile the library
clean_library() {
    echo "Cleaning library"
    
    if [ -d "build/" ] 
    then
        rm -r build
    else 
        echo No build directory found
    fi    
}

# Navigate to the parent directory
cd ..

# Check for arguments
check_arguments "$@"

type=$1
packet=$2

# Check if type is "library" and compile accordingly
if [ "$type" = "library" ]; then
    clean_library
else
    # Check if the packet is valid
    check_valid_packet
    # Compile the packet
    clean_packet
fi
