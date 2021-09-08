#! /usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:

default: all

#
# Makefile
# WARNING: relies on invocation setting current working directory to Makefile location
# This is done in .vscode/task.json
#
PROJECT 			?= lvgl-sdl
MAKEFLAGS 			:= -j $(shell nproc)
SRC_EXT      		:= c
OBJ_EXT				:= o
LIB_EXT				:= a
CC 					?= gcc

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

# Add simulator define to allow modification of source
DEFINES				:= -D SIMULATOR=1 -D LV_BUILD_TEST=0

# Include simulator inc folder first so lv_conf.h from custom UI can be used instead
INC 				:= -I./ui/simulator/inc/ -I./ -I./lvgl/
LDLIBS	 			:= -lSDL2 -lm
BIN 				:= $(BIN_DIR)/demo
LIB 				:= $(BIN_DIR)/lib${PROJECT}.${LIB_EXT}

COMPILE				= $(CC) $(CFLAGS) $(INC) $(DEFINES)

prefix				?= usr/local
libdir				?= ${prefix}/lib
includedir			?= ${prefix}/include
sudo				?=
# Automatically include all source files
SRCS 				:= $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '*/\.*')
OBJECTS    			:= $(patsubst $(SRC_DIR)%,$(BUILD_DIR)/%,$(SRCS:.$(SRC_EXT)=.$(OBJ_EXT)))

all: ${BIN} ${LIB}
	ls $^

$(BUILD_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@echo 'Building project file: $<'
	@mkdir -p $(dir $@)
	@$(COMPILE) -c -o "$@" "$<"

${BIN}: ${LIB}
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $(LDFLAGS) ${LIB} ${LDLIBS}

${LIB}: ${OBJECTS}
	@mkdir -p ${@D}
	${AR} rcs $@ $^

clean:
	rm -rf $(WORKING_DIR)

install: ${BIN} install-headers
	@echo "info: installing to ${DESTDIR} (sudo=${sudo})"
	${sudo} install -d ${DESTDIR}/${libdir}/${PROJECT}/bin/
	${sudo} install $< ${DESTDIR}/${libdir}/${PROJECT}/bin/demo
	${sudo} install -d ${DESTDIR}/${libdir}
	${sudo} install ${LIB} ${DESTDIR}/${libdir}

${DESTDIR}/${includedir}/${PROJECT}:
	${sudo} install -d $@

install-headers: ${DESTDIR}/${includedir}/${PROJECT}
	find . -type f \
	 \( -iname "debian"  -o -iname ".?*" -o -iname "CVS" \) -prune -false \
	 -o -type f -iname "*.h" -print \
	 | while read t; do \
		${sudo} install -vd "$</`dirname $${t}`"; \
		${sudo} install -v "$${t}" "$</$${t}"; \
	done
