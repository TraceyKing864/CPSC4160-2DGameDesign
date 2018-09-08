#ifndef hudMenu_h
#define hudMenu_h

#include <iostream>
#include <vector>
#include <SDL2/sdl.h>
#include "multiGuiElement.h"
#include "unit.h"
#include "HudElement.h"

//this is used to select ability from list
class HudMenu{
private:
  bool hidden;
  std::vector<int> menuItems; //size 4, contains current abilities
  int cur;  //current menuItem
  int locX;
  int locY;
  MultiGuiElement* abilityAttack;     //item 0, up
  MultiGuiElement* abilityDefend;     //1, down
  MultiGuiElement* abilityPrimary;    //2, left
  MultiGuiElement* abilitySecondary;  //3, right
  MultiGuiElement* currentSelection;
  
  HudElement* actionName;
  
public:
  HudMenu();
  ~HudMenu();
  HudMenu(const HudMenu& other);
  HudMenu& operator=(const HudMenu& other);
  void draw();
  void update(Uint32 ticks);

  void open(const Unit& nUnit);
  void close();
  void navigate(direction d);
  int select();
  
  std::string getActionName();
};

#endif
