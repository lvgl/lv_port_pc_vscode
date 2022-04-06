#
# Makefile
# WARNING: relies on invocation setting current working directory to Makefile location
# This is done in .vscode/task.json
#
PROJECT 			?= lvgl-sdl
MAKEFLAGS 			:= -j $(shell nproc)
SRC_EXT      		:= c
OBJ_EXT				:= o
all: CC 			:= gcc
win64: CC 			:= x86_64-w64-mingw32-gcc.exe



SRC_DIR				:= ./
WORKING_DIR			:= ./build
BUILD_DIR			:= $(WORKING_DIR)/obj
BIN_DIR				:= $(WORKING_DIR)/bin
UI_DIR 				:= ui

WARNINGS 			:= -Wall -Wextra \
						-Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wno-discarded-qualifiers \
						-Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized \
						-Wno-unused-parameter -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default  \
					  	-Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated  \
						-Wempty-body -Wshift-negative-value -Wstack-usage=2048 \
            			-Wtype-limits -Wsizeof-pointer-memaccess -Wpointer-arith

CFLAGS 				:= -O0 -g $(WARNINGS)
all: LDFLAGS 		:= 
win64: LDFLAGS 		:= -L./ui/simulator/dlls

# Add simulator define to allow modification of source
all: DEFINES				:= -D SIMULATOR=1 -D LV_BUILD_TEST=0 -D LV_CONF_INCLUDE_SIMPLE=1 -D LV_USE_DEMO_WIDGETS=1
win64: DEFINES				:= -D SIMULATOR=1 -D LV_BUILD_TEST=0 -D LV_CONF_INCLUDE_SIMPLE=1 -D __WIN64__ -D LV_PRId32=PRId32 -D LV_PRIu32=PRIu32 -D LV_USE_DEMO_WIDGETS=1

# Include simulator inc folder first so lv_conf.h from custom UI can be used instead
INC 				:= -I./ui/simulator/inc/ -I./ -I./lvgl/
LDLIBS	 			:= -lSDL2 -lm
BIN 				:= $(BIN_DIR)/demo

COMPILE				= $(CC) $(CFLAGS) $(INC) $(DEFINES)

# Automatically include all source files
SRCS 				:= $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '*/\.*')
OBJECTS    			:= $(patsubst $(SRC_DIR)%,$(BUILD_DIR)/%,$(SRCS:.$(SRC_EXT)=.$(OBJ_EXT)))

all: default

$(BUILD_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@echo 'Building project file: $<'
	@mkdir -p $(dir $@)
	@$(COMPILE) -c -o "$@" "$<"

default: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN) $(OBJECTS) $(LDFLAGS) ${LDLIBS}

clean:
	rm -rf $(WORKING_DIR)

install: ${BIN}
	install -d ${DESTDIR}/usr/lib/${PROJECT}/bin
	install $< ${DESTDIR}/usr/lib/${PROJECT}/bin/

win64: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN) $(OBJECTS) $(LDFLAGS) ${LDLIBS}