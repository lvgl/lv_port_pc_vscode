# CMake最低版本要求
CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

# 项目名称和版本
PROJECT(lvgl-demo VERSION 1.0 LANGUAGES C)

# 设置C标准
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)

# 配置编译选项
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

# 添加警告选项
ADD_COMPILE_OPTIONS(
    -Wall -Wextra
    -Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wno-discarded-qualifiers
    -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing
    -Wno-error=cpp -Wuninitialized -Wno-unused-parameter -Wno-missing-field-initializers
    -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default
    -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic
    -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated
    -Wempty-body -Wshift-negative-value -Wstack-usage=2048
    -Wtype-limits -Wsizeof-pointer-memaccess -Wpointer-arith
)

# 添加宏定义
ADD_DEFINITIONS(-D SIMULATOR=1 -D LV_BUILD_TEST=0)

# 选择显示驱动
SET(LV_DRIVER "SDL2" CACHE STRING "LVGL display driver: SDL2 or X11")
IF(LV_DRIVER STREQUAL "SDL2")
    ADD_DEFINITIONS(-D USE_SDL)
ELSE()
    ADD_DEFINITIONS(-D USE_${LV_DRIVER})
ENDIF()

# 可选库支持
OPTION(USE_FREETYPE "Use FreeType library" OFF)
OPTION(USE_FFMPEG "Use FFmpeg library" OFF)
OPTION(USE_RLOTTIE "Use rlottie library" OFF)

IF(USE_RLOTTIE)
    ADD_DEFINITIONS(-D LV_USE_RLOTTIE=1)
ELSE()
    ADD_DEFINITIONS(-D LV_USE_RLOTTIE=0)
ENDIF()

# 查找SDL2或X11库
IF(LV_DRIVER STREQUAL "SDL2")
    FIND_PACKAGE(SDL2 REQUIRED)
    SET(LIBS SDL2::SDL2)
ELSE()
    FIND_PACKAGE(X11 REQUIRED)
    SET(LIBS X11::X11)
ENDIF()

# 查找FreeType库
IF(USE_FREETYPE)
    FIND_PACKAGE(Freetype REQUIRED)
    LIST(APPEND LIBS Freetype::Freetype)
ENDIF()

# 查找FFmpeg库
IF(USE_FFMPEG)
    FIND_PACKAGE(FFmpeg REQUIRED)
    LIST(APPEND LIBS FFmpeg::FFmpeg)
ENDIF()

# 查找rlottie库
IF(USE_RLOTTIE)
    FIND_PACKAGE(rlottie REQUIRED)
    LIST(APPEND LIBS rlottie::rlottie)
ENDIF()

# 添加系统库依赖
LIST(APPEND LIBS pthread m)

# 查找所有C源文件
FILE(GLOB_RECURSE SOURCES "*.c")

# 过滤掉隐藏文件和构建目录
LIST(FILTER SOURCES EXCLUDE REGEX "\\.\\.")
LIST(FILTER SOURCES EXCLUDE REGEX "build")

# 创建可执行文件
ADD_EXECUTABLE(demo ${SOURCES})

# 链接库
TARGET_LINK_LIBRARIES(demo PRIVATE ${LIBS})

# 包含头文件路径
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/ui/simulator/inc
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lvgl
)

# 安装目标
INSTALL(TARGETS demo DESTINATION /usr/lib/lvgl-demo/bin)

# 自定义清理命令
ADD_CUSTOM_TARGET(clean-all
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMENT "Cleaning all build files"
)