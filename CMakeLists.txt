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
    main/src/mouse_cursor_icon.c
    
    main/src/comm/gui_comm.c
    main/src/comm/gui_comm.h
    main/src/comm/gui_data_comm.c
    main/src/comm/gui_data_comm.h
	
    main/src/connect/connect_main/connect_main.c
    main/src/connect/connect_reset/connect_reset.c
	
    main/src/general/general_lock/general_lock.c
    main/src/general/general_main/general_main.c
    main/src/general/general_shutdown/general_shutdown.c
    main/src/general/general_homescreen/general_homescreen.c
	
    main/src/guide/guide_main/guide_main.c
    main/src/guide/guide_recovery/guide_recovery.c
    main/src/guide/guide_tutorial/guide_tutorial.c
	
    main/src/menu/menu_main.c
	
    main/src/settings/settings_main/settings_main.c
    main/src/settings/settings_about/settings_about.c
    main/src/settings/settings_security/settings_security.c
    main/src/settings/settings_security/settings_reset.c
    main/src/settings/settings_security/settings_erase.c
    main/src/settings/settings_security/settings_complete.c
    main/src/settings/settings_security/settings_pin_changed.c
    main/src/settings/settings_security/settings_change_pin.c
	
    main/src/startup/startup_done/startup_done.c
    main/src/startup/startup_language/startup_language.c
    main/src/startup/startup_pin/startup_set_pin.c
    main/src/startup/startup_pin/startup_enter_pin.c
    main/src/startup/startup_quick_start/startup_quick_start.c
    main/src/startup/startup_ready_check/startup_ready_check.c
    main/src/startup/startup_recovery/startup_recovery.c
    main/src/startup/startup_screen/startup_screen.c
    main/src/startup/startup_verification/startup_verification.c
    main/src/startup/startup_import/startup_import_abort.c
    main/src/startup/startup_import/startup_import_wallet.c
    main/src/startup/startup_import/startup_import_word.c
    main/src/startup/startup_import/startup_import_ready.c
    
    main/src/view/view_transaction/view_transaction_main.c
    main/src/view/view_transaction/view_transaction_confirm.c
    main/src/view/view_transaction/view_transaction_waiting.c
    main/src/view/view_transaction/view_transaction_success.c
    
    main/src/view/view_addr/view_addr_main.c

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
    main/img/img_slider_knob_1.c 
    main/img/img_slider_knob_2.c 
    main/img/img_slider_knob_3.c 
    main/img/img_slider_knob_4.c 
    main/img/img_success.c 
    main/img/img_unchosen.c 
    main/img/img_right_pressed_ffef4b_14x26.c 
    main/img/img_right_released_10e807_14x26.c 
    main/img/img_right_released_f7e100_14x26.c 
    main/img/img_mid_released_f7e100_4x26.c 
    main/img/img_right_pressed_61fc5a_14x26.c 
    main/img/img_mid_released_10e807_4x26.c 
    main/img/img_left_released_10e807_14x26.c 
    main/img/img_left_released_f7e100_14x26.c 
    main/img/img_mid_pressed_61fc5a_4x26.c 
    main/img/img_mid_pressed_ffef4b_4x26.c 
    main/img/img_keypad_back.c 
    main/img/img_keypad_confirm.c 
    main/img/img_left_pressed_61fc5a_14x26.c 
    main/img/img_left_pressed_ffef4b_14x26.c 
    main/img/img_chosen.c 
    main/img/img_alert.c 
    main/img/img_waiting.c 	
    main/img/img_keypad_666666_q.c
    main/img/img_keypad_666666_w.c
    main/img/img_keypad_666666_e.c
    main/img/img_keypad_666666_r.c
    main/img/img_keypad_666666_t.c
    main/img/img_keypad_666666_y.c
    main/img/img_keypad_666666_u.c
    main/img/img_keypad_666666_i.c
    main/img/img_keypad_666666_o.c
    main/img/img_keypad_666666_p.c
    main/img/img_keypad_666666_a.c
    main/img/img_keypad_666666_s.c
    main/img/img_keypad_666666_d.c
    main/img/img_keypad_666666_f.c
    main/img/img_keypad_666666_g.c
    main/img/img_keypad_666666_h.c
    main/img/img_keypad_666666_j.c
    main/img/img_keypad_666666_k.c
    main/img/img_keypad_666666_l.c
    main/img/img_keypad_666666_z.c
    main/img/img_keypad_666666_x.c
    main/img/img_keypad_666666_c.c
    main/img/img_keypad_666666_v.c
    main/img/img_keypad_666666_b.c
    main/img/img_keypad_666666_n.c
    main/img/img_keypad_666666_m.c
    main/img/img_keypad_222222_q.c
    main/img/img_keypad_222222_w.c
    main/img/img_keypad_222222_e.c
    main/img/img_keypad_222222_r.c
    main/img/img_keypad_222222_t.c
    main/img/img_keypad_222222_y.c
    main/img/img_keypad_222222_u.c
    main/img/img_keypad_222222_i.c
    main/img/img_keypad_222222_o.c
    main/img/img_keypad_222222_p.c
    main/img/img_keypad_222222_a.c
    main/img/img_keypad_222222_s.c
    main/img/img_keypad_222222_d.c
    main/img/img_keypad_222222_f.c
    main/img/img_keypad_222222_g.c
    main/img/img_keypad_222222_h.c
    main/img/img_keypad_222222_j.c
    main/img/img_keypad_222222_k.c
    main/img/img_keypad_222222_l.c
    main/img/img_keypad_222222_z.c
    main/img/img_keypad_222222_x.c
    main/img/img_keypad_222222_c.c
    main/img/img_keypad_222222_v.c
    main/img/img_keypad_222222_b.c
    main/img/img_keypad_222222_n.c
    main/img/img_keypad_222222_m.c
    main/img/img_startup_arrow.c    
    main/img/img_screen_shoosen.c    
    main/img/img_name.c


    main/img/img_bg1.c
    main/img/img_bg2.c
    main/img/img_bg3.c
    main/img/img_bg4.c
    main/img/img_bg5.c
    main/img/img_bg6.c    
    main/img/img_bg7.c
    main/img/img_bg1_small.c
    main/img/img_bg2_small.c
    main/img/img_bg3_small.c
    main/img/img_bg4_small.c
    main/img/img_bg5_small.c
    main/img/img_bg6_small.c    
    main/img/img_bg7_small.c
)

include_directories(${PROJECT_SOURCE_DIR}/main/src/comm)

include_directories(${PROJECT_SOURCE_DIR}/main/src/connect/connect_main)
include_directories(${PROJECT_SOURCE_DIR}/main/src/connect/connect_reset)
include_directories(${PROJECT_SOURCE_DIR}/main/src/general/general_lock)
include_directories(${PROJECT_SOURCE_DIR}/main/src/general/general_main)

include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_main)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_recovery)
include_directories(${PROJECT_SOURCE_DIR}/main/src/guide/guide_tutorial)

include_directories(${PROJECT_SOURCE_DIR}/main/src/menu)

include_directories(${PROJECT_SOURCE_DIR}/main/src/settings/settings_about)
include_directories(${PROJECT_SOURCE_DIR}/main/src/settings/settings_main)
include_directories(${PROJECT_SOURCE_DIR}/main/src/settings/settings_security)
include_directories(${PROJECT_SOURCE_DIR}/main/src/settings/settings_reset)

include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_done)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_language)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_pin)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_quick_start)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_ready_check)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_recovery)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_screen)
include_directories(${PROJECT_SOURCE_DIR}/main/src/startup/startup_verification)

include_directories(${PROJECT_SOURCE_DIR}/main/src/view/view_addr)
include_directories(${PROJECT_SOURCE_DIR}/main/src/view/view_transaction)


target_compile_definitions(main PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

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
