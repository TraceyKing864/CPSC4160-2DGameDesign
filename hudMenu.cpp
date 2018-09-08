#include <sstream>
#include <string>
#include "hudMenu.h"
#include "gamedata.h"

HudMenu::HudMenu() : hidden(true), menuItems(), cur(0),
      locX(Gamedata::getInstance().getXmlInt("HUD/actionMenu/posX") < 0 ?
          (Gamedata::getInstance().getXmlInt("view/width") -
          Gamedata::getInstance().getXmlInt("HUD/actionMenu/posX")) :
          Gamedata::getInstance().getXmlInt("HUD/actionMenu/posX")),
      locY(Gamedata::getInstance().getXmlInt("HUD/actionMenu/posY") < 0 ?
          (Gamedata::getInstance().getXmlInt("view/height") +
          Gamedata::getInstance().getXmlInt("HUD/actionMenu/posY")) :
          Gamedata::getInstance().getXmlInt("HUD/actionMenu/posY")),
      abilityAttack(new MultiGuiElement("action_attack",
          (locX + 16), (locY))),
      abilityDefend(new MultiGuiElement("action_defend",
          (locX + 16), (locY + (16 + 8)))),
      abilityPrimary(new MultiGuiElement("action_blank",
          (locX), (locY + (16 - 4)))),
      abilitySecondary(new MultiGuiElement("action_blank",
          (locX + (16 * 2)), (locY + (16 - 4)))),
      currentSelection(new MultiGuiElement("action_select",
          (locX + (16)), (locY))),
      actionName(new HudElement("HUD/actionMenu/element"))
{
  menuItems.reserve(4);
  menuItems.push_back(1);
  menuItems.push_back(2);
  menuItems.push_back(0); //sets extra menu items to a blank action
  menuItems.push_back(0);
  
}
HudMenu::~HudMenu(){
  delete abilityAttack;
  delete abilityDefend;
  delete abilityPrimary;
  delete abilitySecondary;
  delete currentSelection;
}
HudMenu::HudMenu(const HudMenu& other) : hidden(other.hidden), menuItems(other.menuItems),
      cur(other.cur), locX(other.locX), locY(other.locY), abilityAttack(other.abilityAttack),
      abilityDefend(other.abilityDefend), abilityPrimary(other.abilityPrimary),
      abilitySecondary(other.abilitySecondary), currentSelection(other.currentSelection)
{

}
HudMenu& HudMenu::operator=(const HudMenu& other){
  hidden = other.hidden;
  menuItems = other.menuItems;
  cur = other.cur;
  locX = other.locX;
  locY = other.locY;
  abilityAttack = other.abilityAttack;
  abilityDefend = other.abilityDefend;
  abilityPrimary = other.abilityPrimary;
  abilitySecondary = other.abilitySecondary;
  currentSelection = other.currentSelection;
  return *this;
}
void HudMenu::draw(){
  if(!hidden){
    abilityAttack->draw();
    abilityDefend->draw();
    abilityPrimary->draw();
    abilitySecondary->draw();
    currentSelection->draw(); //always draw last
    actionName->draw();
  }
}
void HudMenu::update(Uint32 ticks){
  if(!hidden){
    //only update the currently selected one for animation
    if(cur == 0)
      abilityAttack->update(ticks);
    else if(cur == 1)
      abilityDefend->update(ticks);
    else if(cur == 2)
      abilityPrimary->update(ticks);
    else if(cur == 3)
      abilitySecondary->update(ticks);
    
    currentSelection->update(ticks);
    actionName->update(ticks);
  }
}

void HudMenu::open(const Unit& nUnit){
  int p = nUnit.getPrimary();
  if(p != menuItems[2]){
    menuItems[2] = p;
    int x = abilityPrimary->getX();
    int y = abilityPrimary->getY();
    delete abilityPrimary;
    
    std::string temp;
    temp = "actions/action_" +(std::stringstream() << p).str();
    abilityPrimary = new MultiGuiElement((Gamedata::getInstance().checkTag(temp) ?
          Gamedata::getInstance().getXmlStr(temp) : "action_blank"), x, y);
    if(abilityPrimary->getName() == "action_blank")
      menuItems[2] = 0;
  }
  
  int s = nUnit.getSecondary();
  if(s != menuItems[3]){
    menuItems[3] = s;
    int x = abilitySecondary->getX();
    int y = abilitySecondary->getY();
    delete abilitySecondary;
    
    std::string temp;
    temp = "actions/action_" +(std::stringstream() << s).str();
    abilitySecondary = new MultiGuiElement((Gamedata::getInstance().checkTag(temp) ?
          Gamedata::getInstance().getXmlStr(temp) : "action_blank"), x, y);
    if(abilitySecondary->getName() == "action_blank")
      menuItems[3] = 0;
  }
  
  hidden = false;
  actionName->toggle();
}
void HudMenu::close(){
  cur = 0;  //resets cursor to attack
  currentSelection->setX(abilityAttack->getX());
  currentSelection->setY(abilityAttack->getY());
  hidden = true;
  actionName->toggle();
}
void HudMenu::navigate(direction d){
  if(d == direction::up){
    cur = 0;
    currentSelection->setX(abilityAttack->getX());
    currentSelection->setY(abilityAttack->getY());
  }else if(d == direction::down){
    cur = 1;
    currentSelection->setX(abilityDefend->getX());
    currentSelection->setY(abilityDefend->getY());
  }else if(d == direction::left){
    cur = 2;
    currentSelection->setX(abilityPrimary->getX());
    currentSelection->setY(abilityPrimary->getY());
  }else if(d == direction::right){
    cur = 3;
    currentSelection->setX(abilitySecondary->getX());
    currentSelection->setY(abilitySecondary->getY());
  }
}
int HudMenu::select(){
  return menuItems[cur];
}
std::string HudMenu::getActionName(){
    if(cur == 0)
      return abilityAttack->getName();
    else if(cur == 1)
      return abilityDefend->getName();
    else if(cur == 2)
      return abilityPrimary->getName();
    else if(cur == 3)
      return abilitySecondary->getName();
    else
      return " ";
}
