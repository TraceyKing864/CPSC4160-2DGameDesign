CXX = g++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -g -W -Wall -std=c++14 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_ttf -lSDL2_image -lSDL2_mixer

OBJS = \
  renderContext.o \
	ioMod.o \
	parseXML.o \
	gamedata.o \
	viewport.o \
	combatScene.o \
  combatAi.o \
	board.o \
	cursor.o \
	followSprite.o \
	hudMenu.o \
	multiGuiElement.o \
	tile.o \
	unit.o \
	hud.o \
	guiElement.o \
	hudElement.o \
	spriteSheet.o \
	image.o \
	imageFactory.o \
	frameGenerator.o \
	multiSprite.o \
  explodingSprite.o \
  chunk.o \
  animation.o \
  projectile.o \
	staticSprite.o \
	vector2f.o \
  sound.o \
	clock.o \
	engine.o \
	main.o
EXEC = run

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
	rm -rf frames/*.bmp
