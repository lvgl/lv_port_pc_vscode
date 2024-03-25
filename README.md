# Simulator project for LVGL embedded GUI Library

The [LVGL](https://github.com/lvgl/lvgl) is written mainly for microcontrollers and embedded systems however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

## Requirements
This project is configured for [VSCode](https://code.visualstudio.com) and only tested on Linux, although this may work on OSx or WSL. It requires a working version of GCC, GDB and make in your path.

To allow debugging inside VSCode you will also require a GDB [extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) or other suitable debugger. All the requirements, build and debug settings have been pre-configured in the [.workspace](simulator.code-workspace) file (simply open the project by doubleclick on this file).

The project can use **SDL** but it can be easily relaced by any other built-in LVGL dirvers.

## Usage

### Get the PC project

Clone the PC project and the related sub modules:

```bash
git clone --recursive https://github.com/lvgl/lv_port_pc_vscode
```

### Install graphics driver

Please make sure **SDL**  is installed in the system:

#### Install SDL
You can download SDL from https://www.libsdl.org/

On on Linux you can install it via terminal:
```bash
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
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
