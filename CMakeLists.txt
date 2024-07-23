# 根目录下的CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(lvgl)

option(LV_USE_DRAW_SDL "Use SDL draw unit" OFF)
option(LV_USE_LIBPNG "Use libpng to decode PNG" OFF)
option(LV_USE_LIBJPEG_TURBO "Use libjpeg turbo to decode JPEG" OFF)
option(LV_USE_FFMPEG "Use libffmpeg to display video using lv_ffmpeg" OFF)
option(LV_USE_FREETYPE "Use freetype lib" OFF)

set(CMAKE_C_STANDARD 99)  # C99 # lvgl officially support C99 and above
set(CMAKE_CXX_STANDARD 17)  # C17
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

# 添加 main 目标
add_executable(main 
    main/src/main.c 
    main/src/settings.c 
    main/src/mouse_cursor_icon.c
    main/src/tip.c  # 确保添加 tip.c 文件
    main/src/guide.c  # 确保添加 guide.c 文件
    
    main/src/guide/guide_comm/guide_comm.c
    main/src/guide/guide_connect/guide_connect.c
    main/src/guide/guide_done/guide_done.c
    main/src/guide/guide_general/guide_general.c
    main/src/guide/guide_guide/guide_guide.c
    main/src/guide/guide_language/guide_language.c
    main/src/guide/guide_menu/guide_menu.c
    main/src/guide/guide_pin/guide_set_pin.c
    main/src/guide/guide_pin/guide_enter_pin.c
    main/src/guide/guide_quick_start/guide_quick_start.c
    main/src/guide/guide_ready_check/guide_ready_check.c
    main/src/guide/guide_recovery/guide_recovery.c
    main/src/guide/guide_screen/guide_screen.c
    main/src/guide/guide_settings/guide_settings.c
    main/src/guide/guide_verification/guide_verification.c

    main/img/img_backgroud.c 
    main/img/img_imgbtn_cancle_15x15.c 
    main/img/img_imgbtn_ok_12x9.c 
    main/img/img_left_pressed_8f7ff0_16x40.c 
    main/img/img_left_pressed_bbbbbb_14x26.c 
    main/img/img_left_pressed_bbbbbb_16x40.c 
    main/img/img_left_pressed_ff593c_16x40.c 
    main/img/img_left_released_6c6cf4_14x26.c 
    main/img/img_left_released_6c6cf4_16x40.c 
    main/img/img_left_released_6c6cf4_16x80.c 
    main/img/img_left_released_888888_14x26.c 
    main/img/img_left_released_888888_16x40.c 
    main/img/img_left_released_ff2600_16x40.c 
    main/img/img_menu_connect.c 
    main/img/img_menu_general.c 
    main/img/img_menu_guide.c 
    main/img/img_menu_settings.c 
    main/img/img_mid_pressed_8f7ff0_4x40.c 
    main/img/img_mid_pressed_bbbbbb_4x26.c 
    main/img/img_mid_pressed_bbbbbb_4x40.c 
    main/img/img_mid_pressed_ff593c_4x40.c 
    main/img/img_mid_released_6c6cf4_4x26.c 
    main/img/img_mid_released_6c6cf4_4x40.c 
    main/img/img_mid_released_6c6cf4_16x80.c 
    main/img/img_mid_released_888888_4x26.c 
    main/img/img_mid_released_888888_4x40.c 
    main/img/img_mid_released_ff2600_4x40.c 
    main/img/img_right_pressed_8f7ff0_16x40.c 
    main/img/img_right_pressed_bbbbbb_14x26.c 
    main/img/img_right_pressed_bbbbbb_16x40.c 
    main/img/img_right_pressed_ff593c_16x40.c 
    main/img/img_right_released_6c6cf4_14x26.c 
    main/img/img_right_released_6c6cf4_16x40.c 
    main/img/img_right_released_6c6cf4_16x80.c 
    main/img/img_right_released_888888_14x26.c 
    main/img/img_right_released_888888_16x40.c 
    main/img/img_right_released_ff2600_16x40.c 
    main/img/img_guide_back.c 
    main/img/img_screen_shape.c 
	
)

include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_comm)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_connect)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_done)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_general)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_guide)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_language)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_menu)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_pin)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_quick_start)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_ready_check)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_recovery)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_screen)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_settings)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_verification)

target_compile_definitions(main PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

# 添加 pin 目标
add_executable(pin main/src/pin.c main/src/mouse_cursor_icon.c)

target_compile_definitions(pin PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(pin lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
add_custom_target(run_pin COMMAND ${EXECUTABLE_OUTPUT_PATH}/pin DEPENDS pin)

# 移除 settings 目标
# add_executable(settings main/src/settings.c main/src/mouse_cursor_icon.c)

# target_compile_definitions(settings PRIVATE LV_CONF_INCLUDE_SIMPLE)
# target_link_libraries(settings lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
# add_custom_target(run_settings COMMAND ${EXECUTABLE_OUTPUT_PATH}/settings DEPENDS settings)

# 添加 wallet 目标
add_executable(wallet main/src/wallet.c main/src/mouse_cursor_icon.c)

target_compile_definitions(wallet PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(wallet lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
add_custom_target(run_wallet COMMAND ${EXECUTABLE_OUTPUT_PATH}/wallet DEPENDS wallet)

if(LV_USE_DRAW_SDL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    # Need to install libsdl2-image-dev
    # `sudo apt install libsdl2-image-dev`
    # `brew install sdl2_image`
    find_package(SDL2_image REQUIRED)
    target_include_directories(lvgl PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(main ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries(pin ${SDL2_IMAGE_LIBRARIES})
    # target_link_libraries(settings ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries(wallet ${SDL2_IMAGE_LIBRARIES})
endif(LV_USE_DRAW_SDL)

if(LV_USE_LIBPNG)
    find_package(PNG REQUIRED)
    target_include_directories(lvgl PUBLIC ${PNG_INCLUDE_DIR})
    target_link_libraries(main ${PNG_LIBRARY})
    target_link_libraries(pin ${PNG_LIBRARY})
    # target_link_libraries(settings ${PNG_LIBRARY})
    target_link_libraries(wallet ${PNG_LIBRARY})
endif(LV_USE_LIBPNG)

if(LV_USE_LIBJPEG_TURBO)
    # Need to install libjpeg-turbo8-dev
    # `sudo apt install libjpeg-turbo8-dev`
    # `brew install libjpeg-turbo`
    find_package(JPEG REQUIRED)
    target_include_directories(lvgl PUBLIC ${JPEG_INCLUDE_DIRS})
    target_link_libraries(main ${JPEG_LIBRARIES})
    target_link_libraries(pin ${JPEG_LIBRARIES})
    # target_link_libraries(settings ${JPEG_LIBRARIES})
    target_link_libraries(wallet ${JPEG_LIBRARIES})
endif(LV_USE_LIBJPEG_TURBO)

if(LV_USE_FFMPEG)
    target_link_libraries(main avformat avcodec avutil swscale z)
    target_link_libraries(pin avformat avcodec avutil swscale z)
    # target_link_libraries(settings avformat avcodec avutil swscale z)
    target_link_libraries(wallet avformat avcodec avutil swscale z)
endif(LV_USE_FFMPEG)

if(LV_USE_FREETYPE)
    find_package(Freetype REQUIRED)
    target_link_libraries(main ${FREETYPE_LIBRARIES})
    target_link_libraries(pin ${FREETYPE_LIBRARIES})
    # target_link_libraries(settings ${FREETYPE_LIBRARIES})
    target_include_directories(lvgl PUBLIC ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(wallet ${FREETYPE_LIBRARIES})
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

    target_compile_options(main PRIVATE -fsanitize=address,leak,undefined)
    target_link_options(main PRIVATE -fsanitize=address,leak,undefined)

    target_compile_options(pin PRIVATE -fsanitize=address,leak,undefined)
    target_link_options(pin PRIVATE -fsanitize=address,leak,undefined)

    # target_compile_options(settings PRIVATE -fsanitize=address,leak,undefined)
    # target_link_options(settings PRIVATE -fsanitize=address,leak,undefined)

    target_compile_options(wallet PRIVATE -fsanitize=address,leak,undefined)
    target_link_options(wallet PRIVATE -fsanitize=address,leak,undefined)
endif()
