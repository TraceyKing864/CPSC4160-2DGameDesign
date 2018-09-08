#include <sstream>
#include "hud.h"
#include "gamedata.h"

Hud::Hud() : elements(){
  int size = Gamedata::getInstance().getXmlInt("HUD/size");
  elements.reserve(size);
  for(int i = 0; i < size; i++){
    std::string name = "HUD/element_" + (std::stringstream() << i).str();
    elements.push_back(new HudElement(name));
  }
  actionMenu = new HudMenu();
}
Hud::~Hud(){
  for(std::vector<HudElement*>::iterator it = elements.begin();
        *it != elements.back(); it++){
    delete *it;
  }
  elements.clear();
  delete actionMenu;
}
Hud::Hud(const Hud& other) : elements(other.elements){

}
Hud& Hud::operator=(const Hud& other){
  elements = other.elements;
  return *this;
}

void Hud::toggle(std::string name){
  for(auto it : elements){
    if(it->getName() == name)
      it->toggle();
  }
}
void Hud::toggleHelp(){
  for(auto it : elements){
    if(it->getName() == "help" || it->getName() == "controls")
      it->toggle();
  }
}
void Hud::toggleFps(){
  for(auto it : elements){
    if(it->getName() == "fps")
      it->toggle();
  }
}


void Hud::openActionMenu(const Unit& nUnit){
  actionMenu->open(nUnit);
}
void Hud::closeActionMenu(){
  actionMenu->close();
}
void Hud::navigateActionMenu(direction d){
  actionMenu->navigate(d);
}
int Hud::selectActionMenu(){
  return actionMenu->select();
}
std::string Hud::getActionName(){
  return actionMenu->getActionName();
}

void Hud::update(Uint32 ticks){
  actionMenu->update(ticks);
}
void Hud::draw(){
  for(auto it : elements)
    it->draw();
  actionMenu->draw();
  
}
