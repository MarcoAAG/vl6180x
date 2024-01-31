# Minimum required version of CMake
cmake_minimum_required(VERSION 3.22)

# Project name
project(vl6180x_example)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_SYSTEM_PROCESSOR arm)


# Set msp-gcc for c files
set(CMAKE_C_COMPILER $ENV{HOME}/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER $ENV{HOME}/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++)

set(SOURCES
    ${PROJECT_NAME}.c
    startup.c
    stm32l4xx_it.c
    system_stm32l4xx.c
)

# Include directories
include_directories(
                    ../lib/STM32L4xx_HAL_ST/cmsis
                    ../lib/STM32L4xx_HAL_ST/cmsis/core
                    ../lib/STM32L4xx_HAL_ST/cmsis/device
                    # ../lib/STM32L4xx_HAL_ST/conf
                    # ../lib/STM32L4xx_HAL_ST/stm32l4xx_hal_driver/Inc
                    .
)

# Compiler flags
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -nostartfiles -O2 -g -Wall -std=c11 -Wl,-Map=output.map")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-T ../linker.ld")

add_subdirectory(../lib/STM32L4xx_HAL_ST stm32l4xx_hal_st)

# add_executable(Libraries src/library.c)
add_executable(${PROJECT_NAME}.out ${SOURCES} ${PROJECT_NAME}.c)
target_link_libraries(${PROJECT_NAME}${PROJECT_VERSION}.out 
                        stm32l4xx_hal_st
)
