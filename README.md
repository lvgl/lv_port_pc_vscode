# Simulator project for LVGL embedded GUI Library

The [LVGL](https://github.com/lvgl/lvgl) is written mainly for microcontrollers and embedded systems however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

Using a PC simulator instead of an embedded hardware has several advantages:
* **Costs $0** because you you don't have to buy or design PCB
* **Fast** because you don't have to design an manufacture PCB
* **Collaborative** because any number of developers can work in the same environment
* **Developer friendly** because much easier and faster to debug on PC

## Requirements
This project is configured for VSCode and only tested on Linux, although this may work on OSx or WSL. It requires a working version of GCC, GDB and make in your path.

To allow debugging inside VSCode you will also require the C/C++ Tools extension [Extension](https://code.visualstudio.com/docs/languages/cpp) or other suitable debugger.

* **SDL** a low level driver library to use graphics, handle mouse, keyboard etc.

## Usage

### Get the PC project

Clone the PC project and the related sub modules:

```
git clone --recursive https://github.com/microwavesafe/lv_sim_vscode_sdl
```

### Install SDL
You can download SDL from https://www.libsdl.org/

On on Linux you can install it via terminal:
```
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
```

### Setup
Configure lv_conf.h to suit your project, place your UI code in the UI folder and edit main/src/main.c to call you UI entry function. To build and debug, press F5. You should now have your UI displayed in a new window and can access all the debug features of VSCode through GDB.

To allow temporary modification between simulator and device code, a SIMULATOR=1 define is added globally.
