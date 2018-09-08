#ifndef hudElement_h
#define hudElement_h

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/sdl.h>
#include "guiElement.h"

class HudElement{
private:
  std::string name;
  bool hidden;
  int locX; //location on screen
  int locY;
  int tileWidth;
  int tileHeight;
  int textSpacing;
  int textOffset; //offsets from the first tile
  std::vector<GuiElement*> tiles;
  std::vector<std::string> text;
  
public:
  HudElement(std::string hudID);
  HudElement(const HudElement& other);
  ~HudElement();
  HudElement& operator=(const HudElement& other);
  std::string getName(){return name;}
  
  void toggle();
  
  void update(Uint32 ticks);
  void draw();
  
};

#endif
