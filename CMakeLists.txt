cmake_minimum_required(VERSION 3.10)
project(lvgl)

# FreeRTOS Konfiguration hinzufügen
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/config)
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)
add_definitions(-DFREERTOS_PORT=GCC_POSIX)

# FreeRTOS als Submodule hinzufügen
add_subdirectory(FreeRTOS)

# FreeRTOS-Includes
include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/include)
include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix)
include_directories(${PROJECT_SOURCE_DIR}/config)

# FreeRTOS-Quellen hinzufügen
file(GLOB FREERTOS_SOURCES
    "FreeRTOS/*.c"
    "FreeRTOS/portable/MemMang/heap_4.c"
    "FreeRTOS/portable/ThirdParty/GCC/Posix/*.c"
)

# SquareLineStudio UI files
include_directories(${PROJECT_SOURCE_DIR}/UI)
file(GLOB UI_SOURCES 
    "${PROJECT_SOURCE_DIR}/UI/*.c" 
    "${PROJECT_SOURCE_DIR}/UI/*.h" 
    "${PROJECT_SOURCE_DIR}/UI/screens/ui_Screen1.c"
    "${PROJECT_SOURCE_DIR}/UI/ui.c"
    "${PROJECT_SOURCE_DIR}/UI/components/ui_comp_hook.c"
    "${PROJECT_SOURCE_DIR}/UI/ui_helpers.c"
)

option(LV_USE_DRAW_SDL "Use SDL draw unit" OFF)
option(LV_USE_LIBPNG "Use libpng to decode PNG" OFF)
option(LV_USE_LIBJPEG_TURBO "Use libjpeg turbo to decode JPEG" OFF)
option(LV_USE_FFMPEG "Use libffmpeg to display video using lv_ffmpeg" OFF)
option(LV_USE_FREETYPE "Use freetype lib" OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

find_package(SDL2 REQUIRED SDL2)

add_compile_definitions($<$<BOOL:${LV_USE_DRAW_SDL}>:LV_USE_DRAW_SDL=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBPNG}>:LV_USE_LIBPNG=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBJPEG_TURBO}>:LV_USE_LIBJPEG_TURBO=1>)
add_compile_definitions($<$<BOOL:${LV_USE_FFMPEG}>:LV_USE_FFMPEG=1>)

add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS})

add_executable(main main/src/main.cpp main/src/mouse_cursor_icon.c main/src/FreeRTOS_Posix_Port.c ${FREERTOS_SOURCES} ${UI_SOURCES})

target_compile_definitions(main PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread freertos_config)

add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

if(LV_USE_DRAW_SDL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
    target_include_directories(lvgl PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(main ${SDL2_IMAGE_LIBRARIES})
endif(LV_USE_DRAW_SDL)

if(LV_USE_LIBPNG)
    find_package(PNG REQUIRED)
    target_include_directories(lvgl PUBLIC ${PNG_INCLUDE_DIR})
    target_link_libraries(main ${PNG_LIBRARY})
endif(LV_USE_LIBPNG)

if(LV_USE_LIBJPEG_TURBO)
    find_package(JPEG REQUIRED)
    target_include_directories(lvgl PUBLIC ${JPEG_INCLUDE_DIRS})
    target_link_libraries(main ${JPEG_LIBRARIES})
endif(LV_USE_LIBJPEG_TURBO)

if(LV_USE_FFMPEG)
    target_link_libraries(main avformat avcodec avutil swscale z)
endif(LV_USE_FFMPEG)

if(LV_USE_FREETYPE)
    find_package(Freetype REQUIRED)
    target_link_libraries(main ${FREETYPE_LIBRARIES})
    target_include_directories(lvgl PUBLIC ${FREETYPE_INCLUDE_DIRS})
endif(LV_USE_FREETYPE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lvgl PRIVATE
        -pedantic-errors
        -Wall
        -Wclobbered
        -Wdeprecated
        -Wdouble-promotion
        -Wempty-body
        -Wextra
        -Wformat-security
        -Wmaybe-uninitialized
        # -Wmissing-prototypes
        -Wpointer-arith
        -Wmultichar
        -Wno-pedantic # ignored for now, we convert functions to pointers for propertis table.
        -Wreturn-type
        -Wshadow
        -Wshift-negative-value
        -Wsizeof-pointer-memaccess
        -Wtype-limits
        -Wundef
        -Wuninitialized
        -Wunreachable-code
        -Wfloat-conversion
        -Wstrict-aliasing
    )

    # Check if the compiler supports -fsanitize=leak, -fsanitize=address, and -fsanitize=undefined
    include(CheckCXXCompilerFlag)

    check_cxx_compiler_flag("-fsanitize=leak" HAS_SANITIZE_LEAK)
    check_cxx_compiler_flag("-fsanitize=address" HAS_SANITIZE_ADDRESS)
    check_cxx_compiler_flag("-fsanitize=undefined" HAS_SANITIZE_UNDEFINED)

    # Prepare the list of sanitizers
    set(SANITIZERS "")
    if (HAS_SANITIZE_ADDRESS)
        set(SANITIZERS "${SANITIZERS},address")
    endif()

    if (HAS_SANITIZE_UNDEFINED)
        set(SANITIZERS "${SANITIZERS},undefined")
    endif()

    if (HAS_SANITIZE_LEAK)
        set(SANITIZERS "${SANITIZERS},leak")
    endif()

    # Remove leading comma if there are any sanitizers
    string(REGEX REPLACE "^," "" SANITIZERS "${SANITIZERS}")

    # Add the sanitizers to the compile options if any are supported
    if (SANITIZERS)
        message(STATUS "Sanitizers enabled: ${SANITIZERS}")
        target_compile_options(main PRIVATE -fsanitize=${SANITIZERS})
        target_link_options(main PRIVATE -fsanitize=${SANITIZERS})
    else()
        message(STATUS "Sanitizers not enabled")
    endif()
endif()
