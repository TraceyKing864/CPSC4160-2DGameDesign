#include <sstream>
#include "viewport.h"
#include "ioMod.h"
#include "renderContext.h"  //Used for scale

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(0),
  worldHeight(0),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0), currentPlayer(0),
  unitInfo(NULL), winCond(0),
  objectToTrack(NULL),
  hud()
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() {
  hud.draw();
}

void Viewport::update(Uint32 ticks) {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
  hud.update(ticks);
}

void Viewport::toggleHud(std::string name){
  hud.toggle(name);
}
void Viewport::toggleHelp(){
  hud.toggleHelp();
}
void Viewport::toggleFps(){
  hud.toggleFps();
}

void Viewport::openActionMenu(const Unit& nUnit){
  hud.openActionMenu(nUnit);
}
void Viewport::closeActionMenu(){
  hud.closeActionMenu();
}
void Viewport::navigateActionMenu(direction d){
  hud.navigateActionMenu(d);
}
int Viewport::selectActionMenu(){
  return hud.selectActionMenu();
}
std::string Viewport::getActionName(){
  return hud.getActionName();
}

void Viewport::scale(){
  SDL_RenderSetLogicalSize(RenderContext::getInstance()->getRenderer(), viewWidth, viewHeight);
}
