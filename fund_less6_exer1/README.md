# FUND_LESS6_EXER1: 

## Hardware/Sensors used
These lessons about **I2C peripheral drivers** were done using different sensors than described in the the course. I've used I2C sensors I have with me during the time I've performing this course.

- **Exercise 1: Sensirion SHT25 sensor**
 
   I've implemented a raw communication based on the [sensor datasheet](https://www.sensirion.com/products/catalog/SHT25/). Several MACROS with sensor register addresses were defined and then using Zephyr I2C API functions such as i2c_write_dt() and i2c_write_read_dt() the I2C communication was established with the sensor.

- **Exercise 2: Bosch BNO055 IMU sensor:**
 
    I've integrated the official and open-source [BNO055 C driver](https://github.com/boschsensortec/BNO055_driver/tree/master) developed by Bosch. The bno055_support.c file was used as reference to integrate the sensor C driver with ncs I2C API. 



## Prepare the CMake project file
The file CMakeLists.txt is the main CMake project file and the source of the build process configuration:

Besided the following instructions:

    cmake_minimum_required(VERSION 3.20.0)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
    project(i2c_exercise1)
    target_sources(app PRIVATE src/main.c)

On exercise 2, also add the following instruction to make sure to add the sensor C driver to the build process if the variable **CONFIG_BNO055_SENSOR** is set in Kconfig file:

    target_sources_ifdef(CONFIG_BNO055_SENSOR app PRIVATE src/bno055.c)

## Build and flash Instructions
1. Add this as an existing application in nRF Connect for VS Code extension.
2. Add build configuration and choose the correct board to make sure the correct device-tree files are used.
3. Connect your board and flash the application.