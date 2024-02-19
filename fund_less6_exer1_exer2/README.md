# FUND_LESS6_EXER: 

## Hardware/Sensors used
Lessons 6 about **I2C peripheral drivers** were done using different sensors than described in the the course. I've used I2C sensors I have with me during the time I've performing this course.

- **Exercise 1: Sensirion SHT25 sensor**
 
   I've implemented a raw communication based on the [sensor datasheet](https://www.sensirion.com/products/catalog/SHT25/). Several MACROS with sensor register addresses were defined and then using Zephyr I2C API functions such as i2c_write_dt(), i2c_read_dt() and i2c_write_read_dt() the I2C communication was established with the sensor.

- **Exercise 2: Bosch BNO055 IMU sensor:**
 
    I've integrated the official and open-source [BNO055 C driver](https://github.com/boschsensortec/BNO055_driver/tree/master) developed by Bosch. The bno055_support.c file was used as reference to integrate the sensor C driver with ncs I2C API. 



## Prepare the CMake project file
The file CMakeLists.txt is the main CMake project file and the source of the build process configuration:

Besided the following instructions:

    cmake_minimum_required(VERSION 3.20.0)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
    project(i2c_exercise)
    target_sources(app PRIVATE src/main.c)

On exercise 2 add the **ExternalProject_Add** command to download and build the sensor library during the build process from their own github page. This ensure the project will track the sensor driver repository. Note that ExternalProject_Add command only will be performed if the variable **CONFIG_BNO055_SENSOR** is set in Kconfig file.

In exercise 2, add the **ExternalProject_Add** command to download and build the sensor library during the build process from Bosch BNO055 driver github page. This ensures that the project will track the sensor driver repository. Note that the **ExternalProject_Add** command will only be executed if the variable **CONFIG_BNO055_SENSOR** is defined in the Kconfig file.

    target_sources_ifdef(CONFIG_BNO055_SENSOR app PRIVATE src/bno055.c)

This build-process was set up for learning purposes. A simple way to use the Bosch BNO055 driver would be to download the bno055.c and bno055.h files and place them in the exercise directory. However, this way, if any update or bug fix is ​​made to the BNO055 sensor repository, the exercise will not be configured to track it.

## Build and flash Instructions
1. Add this as an existing application in nRF Connect for VS Code extension.
2. Add build configuration and choose the correct board to make sure the correct device-tree files are used.
3. Connect your board and flash the application.
