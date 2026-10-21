sources  = main.cpp
sources += NTTTBoard.cpp
sources += NTTTPlayerMike.cpp
sources += NTTTPlayerIce.cpp
sources += NTTTManager.cpp
sources += GUI/Button.cpp
sources += GUI/Text.cpp
sources += GUI/TextField.cpp
sources += GUI/Texture.cpp
sources += GUI/RadioButton.cpp
sources += GUI/Screens/BotsScreen.cpp
sources += GUI/Screens/GameScreen.cpp
sources += GUI/Screens/MainMenuScreen.cpp
sources += GUI/Screens/NewGameScreen.cpp

program = nottt

TARGET = linux
#TARGET = windows

objects = $(sources:.cpp=.o)
depends = $(sources:.cpp=.d)
CC = gcc

#DEFINES += -DNDEBUG
DEFINES += -D__USE_STD_IOSTREAM
DEFINES += -std=c++11
DEFINES += -Wall

DEFINES += -g
DEFINES += -O3 

LIBS = -lstdc++ -lSDL2 -lSDL2_image -lSDL2_ttf -lm

ifeq ($(TARGET),linux)
	CC = gcc
	INCLUDE_DIRS = -I/usr/include/SDL2
	LIB_DIRS = 
else
	CC = i686-w64-mingw32-g++
	INCLUDE_DIRS = -I/opt/i686-w64-mingw32/include/SDL2
	LIB_DIRS = -L/opt/i686-w64-mingw32/lib
	DEFINES += -static-libgcc -static-libstdc++
	DEFINES += -DSDL_MAIN_HANDLED
	LIBS += -lSDL2main
	program := $(program).exe
endif


# The tablebase tools use mmap, and are therefore only built on Linux.
ifeq ($(TARGET),linux)
tools = egtb egtb-stats
endif

BINDIR ?= $(HOME)/bin

all: $(program) $(tools)

$(program): $(objects) Makefile
	$(CC) -o $@ $(DEFINES) $(objects) $(LIB_DIRS) $(LIBS)

egtb: egtb.cpp Makefile
	$(CC) -o $@ $(DEFINES) $< -lstdc++

egtb-stats: egtb-stats.cpp Makefile
	$(CC) -o $@ $(DEFINES) $< -lstdc++

install: $(program)
	mkdir -p $(BINDIR)
	cp $(program) $(BINDIR)

%.d: %.cpp
	set -e; $(CC) -M -MT $*.o $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
		| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@; \
		[ -s $@ ] || rm -f $@

include $(depends)

%.o :
	$(CC) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@

clean:
	-rm -f $(objects)
	-rm -f $(depends)
	-rm -f $(program) $(tools) gmon.out

.PHONY: all install clean

