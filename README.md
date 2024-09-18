# VSCode Simulator project for LVGL

[LVGL](https://github.com/lvgl/lvgl) is written mainly for microcontrollers and embedded systems, however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

This project is pre-configured for VSCode and should work work on Windows, Linux and MacOs as well. FreeRTOS is also included and can be optionally enabled to better simulate embedded system's behavior.  


## Get started
### Get the PC project

Clone the PC project and the related sub modules:

```bash
git clone --recursive https://github.com/lvgl/lv_port_pc_vscode
```

### Install SDL and build tools

You can download SDL from https://www.libsdl.org/

#### Linux

Copy below in the Terminal:
For Ubuntu

```bash
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev cmake
```

For ArchLinux

```bash
sudo pacman -Syu && sudo pacman -S sdl2 libsdl2-devel sdl2_mixer sdl2-devel base-devel gcc make
```

## Usage
### Visual Studio Code

1. Be sure you have installed [SDL and the build tools](#install-sdl-and-build-tools)
2. Open the project by double clicking on `simulator.code-workspace` or opening it with `File/Open Workspace from File`
3. Install the recommended plugins
4. Click the Run and Debug page on the left, and select `Debug LVGL demo with gdb` from the drop-down on the top. Like this:
![image](https://github.com/lvgl/lv_port_pc_vscode/assets/7599318/f527b235-5718-4949-b5f0-bd807b3a64ba)
5. Click the Play button or hit F5 to start debugging.

#### ArchLinux User

VSCode does not officially provide an installation package under Arch, you need to use the AUR manager `paru` to install it.
The command is as follows:

```bash
paru -S visual-studio-code-bin
```

#### macOS

Apple's default clang does not support the `-fsanitize=leak` flag.

to build using the latest version of clang from homebrew, do the following:

1. `brew install llvm`

2. cmd+shift+p and run `Cmake: select a kit`, then `[Scan for kits]`

3. then cmd+shift+p and run `Cmake: select a kit`, select the version of clang you just installed from homebrew (it should say `Using compilers C=/opt/homebrew/opt/llvm/bin/clang ...`)

4. reconfigure by running cmd+shift+p `Cmake: Configure`

5. build using [step 4 above](#visual-studio-code)

### FreeRTOS configuration
To correctly configure the project, the RTOS (Real-Time Operating System) requires a significant amount of heap memory, especially when debugging an SDL (Simple DirectMedia Layer) window application. In this project, the heap memory has been experimentally set to **512 MB**.

```c
#define configTOTAL_HEAP_SIZE ( ( size_t ) ( 512 * 1024 * 1024 ) )  // 512 MB Heap
```
This configuration ensures that the SDL window is displayed in a timely manner. If this value is reduced, it may cause significant delays in the SDL window's appearance. If the allocated heap memory is too small, the window may fail to appear altogether.
Therefore, it is crucial to allocate sufficient heap memory to ensure smooth execution and debugging experience.

### Enable FreeRTOS 
To enable the rtos part of this project select in lv_conf.h `#define LV_USE_OS   LV_OS_NONE` to `#define LV_USE_OS  LV_OS_FREERTOS`
Additionaly you have to enable the compilation of all FreeRTOS Files by turn on `option(USE_FREERTOS "Enable FreeRTOS" OFF) ` in the CMakeLists.txt file.

### CMake

This project uses CMake under the hood which can be used without Visula Studio Code too. Just type these in a Terminal when you are in the project's root folder:

```bash
mkdir build
cd build
cmake ..
make -j
```

## Optional library

There are also FreeType and FFmpeg support. You can install these according to the followings:

### Linux

```bash
# FreeType support
wget https://kumisystems.dl.sourceforge.net/project/freetype/freetype2/2.13.2/freetype-2.13.2.tar.xz
tar -xf freetype-2.13.2.tar.xz
cd freetype-2.13.2
make
make install
```

```bash
# FFmpeg support
git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg
cd ffmpeg
git checkout release/6.0
./configure --disable-all --disable-autodetect --disable-podpages --disable-asm --enable-avcodec --enable-avformat --enable-decoders --enable-encoders --enable-demuxers --enable-parsers --enable-protocol='file' --enable-swscale --enable-zlib
make
sudo make install
```
### (RT)OS support
Works with any OS like pthred, Windows, FreeRTOS, etc. It has build in support for FreeRTOS. 

## Test
This project is configured for [VSCode](https://code.visualstudio.com) and is tested on: 
- Ubuntu Linux 
- Windows WSL (Ubuntu Linux)

It requires a working version of GCC, GDB and make in your path.

To allow debugging inside VSCode you will also require a GDB [extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) or other suitable debugger. All the requirements, build and debug settings have been pre-configured in the [.workspace](simulator.code-workspace) file.

The project can use **SDL** but it can be easily relaced by any other built-in LVGL dirvers.
