# FUND_LESS3_EXER2: 

## Customizing an application using CMakeList.txt, kconfig and proj.conf files
The reason for this is to limit the size of your application. Modules and subsystems are only included in the build when you enable the relevant configuration, allowing you to keep the application as small as you wish. 

In this exercise we will customize the build process using CMakeList.txt, kconfig and proj.conf files. 

    cmake_minimum_required(VERSION 3.20.0)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
    project(hello_world)
    target_sources(app PRIVATE src/main.c)
    target_sources_ifdef(CONFIG_MYFUNCTION app PRIVATE src/myfunction.c)

- **target_sources_ifdef**: command to add the custom file myfunction.c to the project if **CONFIG_MYFUNCTION** is enabled:

The CONFIG_MYFUNCTION variable is defined in the kconfig file:

    source "Kconfig.zephyr"

    config MYFUNCTION
        bool "Enable my function"
        default n

and then the CONFIG_MYFUNCTION variable can be enabled/disabled using the proj.conf file:

    CONFIG_MYFUNCTION=y



## Build and flash Instructions
1. Add this as an existing application in nRF Connect for VS Code extension.
2. Add build configuration and choose the correct board to make sure the correct device-tree files are used.
3. Connect your board and flash the application.