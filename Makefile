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

objects = $(sources:.cpp=.o)
depends = $(sources:.cpp=.d)
CC = gcc

#DEFINES += -DNDEBUG
DEFINES += -D__USE_STD_IOSTREAM
DEFINES += -std=c++11
DEFINES += -Wall

DEFINES += -g -pg
#DEFINES += -O3 

nottt: $(objects) Makefile
	$(CC) -o $@ $(DEFINES) $(objects) -lstdc++ -lSDL2 -lSDL2_image -lSDL2_ttf -lm
	mv $@ /home/mike/bin

INCLUDE_DIRS = -I/usr/include/SDL2

%.d: %.cpp
	set -e; $(CC) -M -MT $*.o $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
		| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@; \
		[ -s $@ ] || rm -f $@

include $(depends)

%.o :
	$(CC) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@

clean:
	-rm $(objects)
	-rm $(depends)

