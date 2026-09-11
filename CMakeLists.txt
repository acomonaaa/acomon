cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32F407_LCD_Test)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Core/Src/lcd.c
        Core/Inc/lcd.h
        # 业务隔离层（CubeMX 不会触碰）
        Core/Inc/system_data.h
        Core/Src/system_data.c
        Core/Inc/user_app.h
        Core/Src/user_app.c
        Core/Inc/sensor_app.h
        Core/Src/sensor_app.c
        Core/Inc/control_app.h
        Core/Src/control_app.c
        Core/Inc/app_config.h
        Core/Inc/history_ring.h
        Core/Src/history_ring.c
        Core/Inc/actuator.h
        Core/Src/actuator.c
        Core/Inc/health_app.h
        Core/Src/health_app.c
        Core/Inc/l610_uart.h
        Core/Src/l610_uart.c
        Core/Inc/at_sm.h
        Core/Src/at_sm.c
        Core/Inc/json_mini.h
        Core/Src/json_mini.c
        Core/Inc/mqtt_client.h
        Core/Src/mqtt_client.c
        Core/Inc/cloud_sync.h
        Core/Src/cloud_sync.c)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# LVGL 源文件（递归查找）
file(GLOB_RECURSE LVGL_SRC "lvgl/src/*.c")

# GUI Guider 源文件
set(GUIGUIDER_SRC
    ui/generated/gui_guider.c
    ui/generated/events_init.c
    ui/generated/widgets_init.c
    ui/generated/setup_scr_screen.c
    ui/custom/custom.c
)

# GUI Guider 字体文件
file(GLOB GUIGUIDER_FONTS "ui/generated/guider_fonts/*.c")

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${LVGL_SRC}
    lvgl/porting/lv_port_disp.c
    Core/Src/touch.c
    Core/Src/lv_port_indev.c
    ${GUIGUIDER_SRC}
    ${GUIGUIDER_FONTS}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    lvgl
    lvgl/src
    lvgl/porting
    Core/Inc
    ui/generated
    ui/custom
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)