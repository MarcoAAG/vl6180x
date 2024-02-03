cmake_minimum_required(VERSION 3.22)

project(vl6180x_lib)

set(CMAKE_C_COMPILER $ENV{HOME}/Tools/gcc-arm/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc)


set(HAL_DIR lib/STM32L4xx_HAL_ST)
set(SRC_DIR src)

# include_directories(${HAL_DIR}/Inc)

# Set include directories and sources
set(INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${HAL_DIR}/stm32l4xx_hal_driver/Inc/Legacy
    ${HAL_DIR}/stm32l4xx_hal_driver/Inc
    ${HAL_DIR}/cmsis/device
    ${HAL_DIR}/cmsis/core
    ${HAL_DIR}/cmsis
    ${HAL_DIR}/conf
)

set(SOURCES
    ${SRC_DIR}/${PROJECT_NAME}.c
)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Configure library compilation options
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# Configure additional compilation options, if needed
target_compile_options(${PROJECT_NAME} PRIVATE 
                                        -c 
                                        -mcpu=cortex-m4
                                        -std=c11 
                                        -Wall
                                        -mthumb
                                        )

# Configure linking options, if needed
# target_link_libraries(${PROJECT_NAME} ...)

# Configure installation options, if needed
# install(TARGETS ${PROJECT_NAME} DESTINATION lib)
# install(FILES ... DESTINATION include)