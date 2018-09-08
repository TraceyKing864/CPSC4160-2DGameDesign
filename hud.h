#ifndef HUD_h
#define HUD_h

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "hudElement.h"
#include "hudMenu.h"

class Hud{
private:
  std::vector<HudElement*> elements;  //non interactive
  HudMenu* actionMenu;
  
public:
  Hud();
  ~Hud();
  Hud(const Hud& other);
  Hud& operator=(const Hud& other);
  
  void toggle(std::string name);
  void toggleHelp();
  void toggleFps();
  
  void openActionMenu(const Unit& nUnit);
  void closeActionMenu();
  void navigateActionMenu(direction d);
  int selectActionMenu();
  std::string getActionName();
  
  void update(Uint32 ticks);
  void draw();
  
};


#endif
