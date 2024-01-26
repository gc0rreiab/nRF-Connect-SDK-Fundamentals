# FUND_LESS3_EXER1: 

## Prepare the CMake project file
The file CMakeLists.txt is the main CMake project file and the source of this build process configuration:

    cmake_minimum_required(VERSION 3.20.0)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
    project(hello_world)
    target_sources(app PRIVATE src/main.c)

- **cmake_minimum_required**: command is used to ensures the build fails if the CMake version is too old.
-  **find_package** command is used to find and load settings for an external package.
    - Zephyr: This is the name of the package being searched for, in this case, the Zephyr RTOS.
    - REQUIRED: This keyword indicates that the package must be found. If the package is not found, it will result in an build error.
    - HINTS $ENV{ZEPHYR_BASE}: The HINTS option is used to provide additional search paths or hints to help CMake locate the package. This part provides a hint to CMake about where it should look for the Zephyr package. $ENV{ZEPHYR_BASE} is an environment variable that specifies the base directory of the Zephyr installation. 
- **project**: command to set the name of the project.
- **target_sources**: command to add the source file to the project.


## Build and flash Instructions
1. Add this as an existing application in nRF Connect for VS Code extension.
2. Add build configuration and choose the correct board to make sure the correct device-tree files are used.
3. Connect your board and flash the application.
 
