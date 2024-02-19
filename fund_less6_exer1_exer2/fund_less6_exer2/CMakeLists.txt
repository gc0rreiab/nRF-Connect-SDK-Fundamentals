cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(i2c_exercise2)
target_sources(app PRIVATE src/main.c)

include(ExternalProject)

#Set the directory where the external project will be downloaded and built
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

#Define the ExternalProject for BN055 driver
ExternalProject_Add(
    BNO055_driver
    PREFIX ${EXTERNAL_INSTALL_LOCATION}/BNO055_driver
    GIT_REPOSITORY https://github.com/boschsensortec/BNO055_driver.git
    GIT_TAG master # or specify a specific tag or commit
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

# Add the root directory of the BNO055 driver to the include directories
ExternalProject_Get_Property(BNO055_driver source_dir)
include_directories(${source_dir})
# Optionally, you can add a dependency on BNO055_driver to ensure that 'BNO055_driver' is built before the project target 'app'
add_dependencies(app BNO055_driver)


