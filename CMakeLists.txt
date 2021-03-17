cmake_minimum_required(VERSION 3.13)

project(lvgl_simulator)

set(LVGL_SIMULATOR_SDL_DIR "path-to-sdl-dir" CACHE STRING "Path to the SDL directory")
set(SDL_INCLUDE_DIR ${LVGL_SIMULATOR_SDL_DIR}/include)
set(SDL_LIB_DIR ${LVGL_SIMULATOR_SDL_DIR}/lib)
set(SDL_BIN_DIR ${LVGL_SIMULATOR_SDL_DIR}/bin)

add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(lv_examples)
target_include_directories(lv_examples PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(lv_examples lvgl)

add_subdirectory(lv_drivers)
target_include_directories(lv_drivers PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(lv_drivers PUBLIC ${SDL_INCLUDE_DIR} )

add_executable(lv_simulator 
    main/src/main.c 
    main/src/mouse_cursor_icon.c
)
target_link_libraries(lv_simulator lvgl lv_examples lv_drivers SDL2)
target_link_directories(lv_simulator PRIVATE ${SDL_LIB_DIR})

# Copy SDL2 DLL to the binary dir to avoid adding SDL dir to PATH.
add_custom_command(TARGET lv_simulator POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SDL_BIN_DIR}/SDL2.dll ${CMAKE_BINARY_DIR}/. )