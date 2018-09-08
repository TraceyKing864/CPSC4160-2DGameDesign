#include "guiElement.h"
#include "gamedata.h"
#include "renderContext.h"
#include "viewport.h"

GuiElement::GuiElement( const std::string& name, int nX, int nY) :
  Drawable(name, Vector2f(nX, nY), Vector2f(0,0)),
  images(RenderContext::getInstance()->getImages(name)),
  type(4), //hard coded for now to be the default type
  rows(Gamedata::getInstance().getXmlInt(name+"/rows")),
  cols(Gamedata::getInstance().getXmlInt(name+"/cols"))
{
// added this code to scale objects
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
}
GuiElement::GuiElement( const std::string& name, int nX, int nY, int nType) :
  Drawable(name, Vector2f(nX, nY), Vector2f(0,0)),
  images(RenderContext::getInstance()->getImages(name)),
  type(nType),
  rows(Gamedata::getInstance().getXmlInt(name+"/rows")),
  cols(Gamedata::getInstance().getXmlInt(name+"/cols"))
{
// added this code to scale objects
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
}

GuiElement::GuiElement(const GuiElement& s) :
  Drawable(s),
  images(s.images),
  type(s.type),
  rows(s.rows),
  cols(s.cols)
  { }

GuiElement& GuiElement::operator=(const GuiElement& s) {
  Drawable::operator=(s);
  images = (s.images);
  type = (s.type);
  rows = (s.rows);
  cols = (s.cols);
  return *this;
}

void GuiElement::draw() const {
  images[type]->draw((getX() + Viewport::getInstance().getX()), (getY() + Viewport::getInstance().getY()), getScale());
}

void GuiElement::update(Uint32 ticks) {
  //Intentionally left empty
}
