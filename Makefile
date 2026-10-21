# Game sources, relative to src/. Object and dependency files go to build/.
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

objects = $(addprefix build/,$(sources:.cpp=.o))
depends = $(objects:.o=.d)
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
# They keep their asserts even with -DNDEBUG above: the checks cost no
# measurable time, and the tools are only run offline.
ifeq ($(TARGET),linux)
tools = egtb egtb-stats
endif

BINDIR ?= $(HOME)/bin

help_line = @printf "  make %-11s %s\n"

help:
	@echo "Targets:"
	$(help_line) help "Show this list (the default)"
	$(help_line) all "Build $(strip $(program) $(tools))"
	$(help_line) $(program) "Build the game (GUI and tournament mode)"
ifeq ($(TARGET),linux)
	$(help_line) egtb "Build the tablebase generator"
	$(help_line) egtb-stats "Build the tablebase statistics tool"
endif
	$(help_line) install "Copy $(program) to $(BINDIR) (override with BINDIR=...)"
	$(help_line) clean "Remove all build output"

all: $(program) $(tools)

$(program): $(objects) Makefile
	$(CC) -o $@ $(DEFINES) $(objects) $(LIB_DIRS) $(LIBS)

egtb: tools/egtb.cpp Makefile
	$(CC) -o $@ $(filter-out -DNDEBUG,$(DEFINES)) $< -lstdc++

egtb-stats: tools/egtb-stats.cpp Makefile
	$(CC) -o $@ $(filter-out -DNDEBUG,$(DEFINES)) $< -lstdc++

install: $(program)
	mkdir -p $(BINDIR)
	cp $(program) $(BINDIR)

build/%.d: src/%.cpp Makefile
	@mkdir -p $(@D)
	set -e; $(CC) -M -MT build/$*.o $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
		| sed 's#\(build/$*\)\.o[ :]*#\1.o $@ : #g' > $@; \
		[ -s $@ ] || rm -f $@

# The dependency files are only needed when building, not for help or clean.
ifneq ($(filter-out help clean,$(MAKECMDGOALS)),)
include $(depends)
endif

build/%.o: src/%.cpp Makefile
	$(CC) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@

clean:
	-rm -rf build
	-rm -f $(program) $(tools) gmon.out

.PHONY: help all install clean

