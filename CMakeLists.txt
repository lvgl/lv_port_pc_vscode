cmake_minimum_required(VERSION 3.18)

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

include_directories(
    ${SDL2_INCLUDE_DIRS}
)

# Generate compile_commands.json file 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wshadow
    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:DEBUG>:-Og>
    $<$<CONFIG:RELEASE>:-Os>  
)

add_compile_definitions(
    LV_BUILD_TEST=0
    SIMULATOR=1
    $<$<CONFIG:DEBUG>:DEBUG>
)


include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/main/src
    ${CMAKE_SOURCE_DIR}/lvgl
    ${CMAKE_SOURCE_DIR}/lv_examples
)

add_subdirectory(lv_drivers)
add_subdirectory(lvgl)
add_subdirectory(lv_examples)

# list of modules to build the appliaction (without extension .c or .cpp)
add_executable(${PROJECT_NAME}
    main/src/main.c
    main/src/mouse_cursor_icon.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lv_drivers lv_examples ${SDL2_LIBRARY})
