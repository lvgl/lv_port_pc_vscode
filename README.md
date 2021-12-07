# Simulator project for LVGL embedded GUI Library

The [LVGL](https://github.com/lvgl/lvgl) is written mainly for microcontrollers and embedded systems however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

Using a PC simulator instead of an embedded hardware has several advantages:
* **Costs $0** because you you don't have to buy or design PCB
* **Fast** because you don't have to design and manufacture PCB
* **Collaborative** because any number of developers can work in the same environment
* **Developer friendly** because much easier and faster to debug on PC

## Requirements
This project is configured for VSCode and only tested on Linux, although this may work on macOS (see below) or WSL. It requires a working version of GCC, GDB and make in your path.

To allow debugging inside VSCode you will also require a GDB [extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) or other suitable debugger.

* **SDL** a low level driver library to use graphics, handle mouse, keyboard etc.

## Usage

### Get the PC project

Clone the PC project and the related sub modules:

```
git clone --recursive https://github.com/lvgl/lv_sim_vscode_sdl
```

### Install SDL
You can download SDL from https://www.libsdl.org/

On on Linux you can install it via terminal:
```
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
```

On macOS you can install it using [Homebrew](https://brew.sh/) via the terminal:
```
brew install sdl2
```

### Setup
To allow custom UI code an `lv_conf.h` file placed at `ui/simulator/inc` will automatically override this projects lv_conf.h file. By default code under `ui` is ignored so you can reuse this repository for multiple projects. You will need to place a call from `main.c` to your UI's entry function.

To build and debug, press F5. You should now have your UI displayed in a new window and can access all the debug features of VSCode through GDB.

To allow temporary modification between simulator and device code, a SIMULATOR=1 define is added globally.


## CMake

The project can be built using CMake, either within VS Code or from the command line.

### VS Code CMake

Install one of the many CMake extensions, e.g. [twxs.cmake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake). This will detect the `CMakeLists.txt` file and configure the project accordingly. 

The executable can be invoked using:
```
./build/lvgl-sdl 
```

### Command Line Interface (CLI)

Use the build script `build.sh` to easily build (and rebuild the project)
```
./build.sh --help
Usage: build.sh [-v | --verbose] [ reset | clean | debug | release ]
```
* `reset` removes the build directory and reconfugures the project from first principles. It is always best to start with a `reset` build

Run the executable at:
```
./build/debug/lvgl-sdl 
```

## macOS

This project has been tested with `Monterey`, Clang v13.0.0, SDL2 2.0.18
```
$ gcc -v
Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/4.2.1
Apple clang version 13.0.0 (clang-1300.0.29.3)
Target: x86_64-apple-darwin21.1.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```
### macOS SDL2 CMake "Hack" (YMMV)

CMake happily finds the package SDL2, but is not setting the correct paths and library includes (to be resolved). The stock [stackoverflow](https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/) answer did not fix the issue (outstanding issue to be resolved).

Current the `hack` is to hard-code the include paths in the `CMakeLists.txt`.
```
# if MacOS then hard code paths to get SDL linking
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SDL2_INCLUDE_DIR /usr/local/Cellar/sdl2/2.0.18/include/)
    link_directories(/usr/local/Cellar/sdl2/2.0.18/lib/)
    set(SDL2_LIBRARY SDL2)
endif()
```

This path may need editing for your install of SDL2. However if installed using [Homebrew](https://brew.sh/) then it is likely to be correct for the current version of SDL2.

To check for the path of SDL2 using homebrew:
```
brew ls SDL2
/usr/local/Cellar/sdl2/2.0.18/bin/sdl2-config
/usr/local/Cellar/sdl2/2.0.18/include/SDL2/ (77 files)
/usr/local/Cellar/sdl2/2.0.18/lib/libSDL2-2.0.0.dylib
/usr/local/Cellar/sdl2/2.0.18/lib/cmake/ (2 files)
/usr/local/Cellar/sdl2/2.0.18/lib/pkgconfig/sdl2.pc
/usr/local/Cellar/sdl2/2.0.18/lib/ (4 other files)
/usr/local/Cellar/sdl2/2.0.18/share/aclocal/sdl2.m4
```
If not installed, then install using `brew`:
```
brew install sdl2
```
