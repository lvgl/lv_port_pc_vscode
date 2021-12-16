cmake_minimum_required(VERSION 3.18)
include(FetchContent)

# project name
project(lvgl-sdl LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
  message("SDL2 inc ${SDL2_INCLUDE_DIRS}")
  message("SDL2 lib ${SDL2_LIBRARY}")
endif()

include_directories(${SDL2_INCLUDE_DIRS})

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Specify path to own LVGL config header
set(LV_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h
    CACHE STRING "" FORCE)

# Fetch LVGL from GitHub
FetchContent_Declare(
  lvgl 
  GIT_REPOSITORY https://github.com/lvgl/lvgl.git
  # GIT_TAG d38eb1e689fa5a64c25e677275172d9c8a4ab2f0
  # GIT_TAG        v8.1.0
)

# Specify path to own LVGL demos config header
set(LV_DEMO_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_demo_conf.h
    CACHE STRING "" FORCE)

# Fetch LVGL Drivers from GitHub
FetchContent_Declare(lv_drivers
                     GIT_REPOSITORY https://github.com/lvgl/lv_drivers)

# Fetch LVGL Demos from GitHub
FetchContent_Declare(lv_demos
                     GIT_REPOSITORY https://github.com/lvgl/lv_demos.git)

FetchContent_MakeAvailable(lvgl lv_drivers lv_demos)


add_compile_options(
  -ffunction-sections
  -fdata-sections
  -Wall
  -Wshadow
  $<$<CONFIG:DEBUG>:-g3>
  $<$<CONFIG:DEBUG>:-Og>
  $<$<CONFIG:RELEASE>:-Os>)

add_compile_definitions(LV_BUILD_TEST=0 SIMULATOR=1 $<$<CONFIG:DEBUG>:DEBUG>)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/main/src)

# list of modules to build the appliaction (without extension .c or .cpp)
add_executable(${PROJECT_NAME} main/src/main.c main/src/mouse_cursor_icon.c)

target_link_libraries(${PROJECT_NAME} PRIVATE lvgl::demos lvgl::lvgl lv_drivers
                                              ${SDL2_LIBRARY})

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  # using Clang
  target_link_options(
    ${PROJECT_NAME} PRIVATE
    LINKER:-map,${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  target_link_options(
    ${PROJECT_NAME} PRIVATE
    LINKER:-Map,${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map)
endif()
