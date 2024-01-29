# FUND_LESS3_EXER2: 

## Customizing an application using CMakeList.txt, kconfig and proj.conf files
The reason for this is to limit the size of your application. Modules and subsystems are only included in the build when you enable the relevant configuration, allowing you to keep the application as small as you wish. 

In this exercise the build process was customized using the CMakeList.txt, kconfig and proj.conf files.

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

## Customizing the hardware using device-tree overlays
Additionally, hardware can be customized, but it is good practice to use device-tree overlay files rather than directly editing the board's device tree-file to ensure that the file used for other applications is not changed. To do this, a file must be created with the same name as the board and with the .overlay extension.

In the exercise, only the baud rate of the uart0 instance, which is used as the board console, is changed.

    &uart0 {
        current-speed = < 9600 >;
    };



## Build and flash Instructions
NOTE: Regardless of the method used, you have to use the pristine build option whenever you have made changes to your input files (as opposed to application files / source code files), such as the prj.conf, to make sure that these changes are
included in the new build. If you are only making changes to your source code then you can just use the regular non-pristine build option.

### Using nRF Connect for VS Code extension
1. Add this as an existing application in nRF Connect for VS Code extension
2. Add build configuration and choose the correct board to make sure the correct device-tree files are used and the device-tree overlay file is added.
3. Connect your board and flash the application.

### Using West tool
1. Locate your application path
2. Build your application using the following command:

        west build --build-dir <custom_build_directory> -b <board_name> --pristine
3. Connect your board and flash the application.
