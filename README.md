# Simulator project for LVGL embedded GUI Library

The [LVGL](https://github.com/lvgl/lvgl) is written mainly for microcontrollers and embedded systems however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

Using a PC simulator instead of an embedded hardware has several advantages:
* **Costs $0** because you don't have to buy or design PCB
* **Fast** because you don't have to design and manufacture PCB
* **Collaborative** because any number of developers can work in the same environment
* **Developer friendly** because much easier and faster to debug on PC

## Requirements
This project is configured for [VSCode](https://code.visualstudio.com) and only tested on Linux and macOS, although this may work on WSL as well. It requires a working version of GCC, GDB and make in your path.

To allow debugging inside VSCode you will also require a GDB [extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) or other suitable debugger. All the requirements have been pre-configured in the [.workspace](simulator.code-workspace) file (simply open the project by doubleclick on this file).

The project can use **SDL** or **X11** as LVGL display driver for lowlevel graphics/mouse/keyboard support. This can be defined in the [Makefile](Makefile#L8).
Please make sure the selected library is installed in the system (check [Install graphics driver](#install-graphics-driver)).

## Usage

### Get the PC project

Clone the PC project and the related sub modules:

```bash
git clone --recursive https://github.com/lvgl/lv_port_pc_vscode
```

### Install graphics driver
The project can use **SDL** or **X11** as LVGL display driver. This can be selected in the [Makefile](Makefile#L8).
Please make sure the used library is installed in the system:

### Install SDL
You can download SDL from https://www.libsdl.org/

On Linux you can install it via terminal:
```bash
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
```

On macOS you can install it via terminal:
```bash
brew install sdl2
```

### Install X11
On Linux you can install it via terminal:
```bash
sudo apt-get update && sudo apt-get install -y libx11-dev
```

### Optional library
There are also FreeType and FFmpeg support. You can install FreeType support with:
```bash
# FreeType support
wget https://kumisystems.dl.sourceforge.net/project/freetype/freetype2/2.13.2/freetype-2.13.2.tar.xz
tar -xf freetype-2.13.2.tar.xz
cd freetype-2.13.2
make
make install
```

The FFmpeg support can be installed with:
```bash
# FFmpeg support
git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg
cd ffmpeg
git checkout release/6.0
./configure --disable-all --disable-autodetect --disable-podpages --disable-asm --enable-avcodec --enable-avformat --enable-decoders --enable-encoders --enable-demuxers --enable-parsers --enable-protocol='file' --enable-swscale --enable-zlib
make
sudo make install
```

## Building

### With SDL2

Select **SDL2** at the beginning of the Makefile:

```Makefile
# select underlaying LCGL display driver (SDL2 || X11)
# LV_DRIVER          := X11
LV_DRIVER          := SDL2
```

#### Notes for macOS

On macOS you'll need to include the [SDL2 library](https://formulae.brew.sh/formula/sdl2). To find the include path, run via terminal:

```bash
brew ls SDL2
```

It will print something like this:

```bash
/opt/homebrew/Cellar/sdl2/2.28.5/bin/sdl2-config
/opt/homebrew/Cellar/sdl2/2.28.5/include/SDL2/ (78 files)
/opt/homebrew/Cellar/sdl2/2.28.5/lib/libSDL2-2.0.0.dylib
/opt/homebrew/Cellar/sdl2/2.28.5/lib/cmake/ (2 files)
/opt/homebrew/Cellar/sdl2/2.28.5/lib/pkgconfig/sdl2.pc
/opt/homebrew/Cellar/sdl2/2.28.5/lib/ (4 other files)
/opt/homebrew/Cellar/sdl2/2.28.5/share/aclocal/sdl2.m4
```

You take the second line path and copy into the Makefile:

```Makefile
INC    := -I./ui/simulator/inc/ -I./ -I./lvgl/ -I/opt/homebrew/Cellar/sdl2/2.28.5/include # -I/usr/include/freetype2 -L/usr/local/lib
```

Next, include the SDL2 library and respective C flags, by adding `sdl2-config --cflags --libs`:

```Makefile
$(BIN): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN) $(OBJECTS) $(LDFLAGS) ${LDLIBS} `sdl2-config --cflags --libs`
```

Finally, change the `main.c` include `#include <SDL2/SDL.h>` to `#include <SDL.h>`.

### Build with **X11**:

Select X11 at the beginning of the Makefile:

```Makefile
# select underlaying LCGL display driver (SDL2 || X11)
LV_DRIVER          := X11
# LV_DRIVER          := SDL2
```

This is enough to build. Try via terminal:

```bash
make
```

### Setup
To allow custom UI code an `lv_conf.h` file placed at `ui/simulator/inc` will automatically override this projects lv_conf.h file. By default code under `ui` is ignored so you can reuse this repository for multiple projects. You will need to place a call from `main.c` to your UI's entry function.

To build and debug, press F5. You should now have your UI displayed in a new window and can access all the debug features of VSCode through GDB.

To allow temporary modification between simulator and device code, a SIMULATOR=1 define is added globally.
