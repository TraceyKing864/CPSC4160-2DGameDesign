#include <sstream>
#include "hudElement.h"
#include "gamedata.h"
#include "ioMod.h"
#include "clock.h"
#include "viewport.h"

HudElement::HudElement(std::string hudID) :
      name(Gamedata::getInstance().getXmlStr(hudID+"/name")),
      hidden(Gamedata::getInstance().getXmlBool(hudID+"/hidden")),
      locX(Gamedata::getInstance().getXmlInt(hudID+"/posX")),
      locY(Gamedata::getInstance().getXmlInt(hudID+"/posY")),
      tileWidth(Gamedata::getInstance().getXmlInt(hudID+"/tiled_width")),
      tileHeight(Gamedata::getInstance().getXmlInt(hudID+"/tiled_height")),
      textSpacing(Gamedata::getInstance().getXmlInt("HUD/textSpacing")),
      textOffset(Gamedata::getInstance().getXmlInt("HUD/textOffset")),
      tiles(), text()
{
  //Negative means offset from the edges of the view
  if(locX < 0)
    locX = Gamedata::getInstance().getXmlInt("view/width") + locX;
  if(locY < 0)
    locY = Gamedata::getInstance().getXmlInt("view/height") + locY;
  tiles.reserve(tileWidth * tileHeight);
  for(int j = 0; j < tileHeight; j++){
    for(int i = 0; i < tileWidth; i++){
      int type = 0;
      if(j == 0){
        if(i == 0){                         //top left
          type = 0;
        }else if(i == (tileWidth-1)){       //top right
          type = 2;
        }else{                              //top middle
          type = 1;
        }
      }else if(j == (tileHeight-1)){
        if(i == 0){                         //bottom left
          type = 6;
        }else if(i == (tileWidth-1)){       //bottom right
          type = 8;
        }else{                              //bottom middle
          type = 7;
        }
      }else{
        if(i == 0){                         //left
          type = 3;
        }else if(i == (tileWidth-1)){       //right
          type = 5;
        }else{                              //middle
          type = 4;
        }
      }
      tiles.push_back(new GuiElement(Gamedata::getInstance().getXmlStr(hudID + "/tile"),
          (locX + (i * 16)), (locY + (j * 16)), type));
    }
    
  }
  int textNum = Gamedata::getInstance().getXmlInt(hudID+"/text");
  text.reserve(textNum);
  for(int i = 0; i < textNum; i++){
    std::string name = hudID+"/text_"+(std::stringstream() << i).str();
    text.push_back(Gamedata::getInstance().getXmlStr(name));
  }
  
}
HudElement::~HudElement(){
  for(std::vector<GuiElement*>::iterator it = tiles.begin();
        *it != tiles.back(); it++){
    delete *it;
  }
  tiles.clear();
}
void HudElement::toggle(){
  hidden = !hidden;
}
void HudElement::update(Uint32 ticks){
  //intentionally left blank
}
void HudElement::draw(){
  if(!hidden){
    for(auto it : tiles)
      it->draw();
    for(int i = 0; i < text.size(); i++){
      if(text[i] == "fps()"){
        std::stringstream screenText;
        screenText.str("");
        screenText << Clock::getInstance().getFps();
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "tracking()"){
        std::stringstream screenText;
        screenText.str("");
        screenText << Viewport::getInstance().getObjectToTrack()->getName();
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "turn()"){
        std::stringstream screenText;
        screenText.str("");
        int p = Viewport::getInstance().getPlayer();
        if(p == 0)      //player turn
          screenText << "your turn";
        else if(p == 1) //AI turn
          screenText << "AI turn";
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "gameOver()"){
        std::stringstream screenText;
        screenText.str("");
        int wc = Viewport::getInstance().getWinCond();
        if(wc == 1)      //player turn
          screenText << "You Win! :)";
        else if(wc == 2) //AI turn
          screenText << "You Lost. :(";
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "unitName()"){
        std::stringstream screenText;
        screenText.str("");
        Unit* u = Viewport::getInstance().getUnitInfo();
        if(u)
          screenText << u->getName();
        else
          screenText << " ";
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "unitHp()"){
        std::stringstream screenText;
        screenText.str("");
        Unit* u = Viewport::getInstance().getUnitInfo();
        if(u)
          screenText << "HP: " << u->getCurhp() << " / " << u->getHp();
        else
          screenText << " ";
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "unitMp()"){
        std::stringstream screenText;
        screenText.str("");
        Unit* u = Viewport::getInstance().getUnitInfo();
        if(u)
          screenText << "MP: " << u->getCurmp() << " / " << u->getMp();
        else
          screenText << " ";
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }else if(text[i] == "actionName()"){
        std::stringstream screenText;
        screenText.str("");
        std::string s = Viewport::getInstance().getActionName();
        screenText << s;
        IoMod::getInstance().writeText(screenText.str(), locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }
      
      
      else{
        IoMod::getInstance().writeText(text[i], locX + textOffset, (locY + textOffset + (i * textSpacing)));
      }
    }
  }
}

