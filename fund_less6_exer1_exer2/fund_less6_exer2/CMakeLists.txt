# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
include(FetchContent)

FetchContent_Declare(
    BNO055_driver
    GIT_REPOSITORY https://github.com/boschsensortec/BNO055_driver.git
    GIT_TAG        master # release-1.10.0
)

# Get the source directory of the external project
FetchContent_GetProperties(BNO055_driver)
FetchContent_MakeAvailable(BNO055_driver)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(MyProject)

# Add your application source files
target_sources(app PRIVATE src/main.c)

# Include the directory containing the header file
zephyr_include_directories(${bno055_driver_SOURCE_DIR})
# add the c-file
target_sources(app PRIVATE ${bno055_driver_SOURCE_DIR}/bno055.c)