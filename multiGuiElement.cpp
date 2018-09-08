#include "multiGuiElement.h"
#include "gamedata.h"
#include "renderContext.h"
#include "viewport.h"

void MultiGuiElement::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

MultiGuiElement::MultiGuiElement( const std::string& name, int nX, int nY) :
  Drawable(name, Vector2f(nX, nY), Vector2f(0.0, 0.0)),
  images( RenderContext::getInstance()->getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  currentVariation(0),
  numberOfVariations( Gamedata::getInstance().getXmlInt(name+"/variations") ),
  frameInterval( Gamedata::getInstance().checkTag(name+"/frameInterval") ?
        Gamedata::getInstance().getXmlInt(name+"/frameInterval") :
        Gamedata::getInstance().getXmlInt("sprite_default/frameInterval")),
  timeSinceLastFrame( 0.0 )
{
  if(Gamedata::getInstance().checkTag(name+"/name"))
    setName(Gamedata::getInstance().getXmlStr(name+"/name"));
  // added this code to scale objects
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
}

MultiGuiElement::MultiGuiElement(const MultiGuiElement& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  currentVariation(s.currentVariation),
  numberOfVariations(s.numberOfVariations),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame )
  { }

MultiGuiElement& MultiGuiElement::operator=(const MultiGuiElement& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  currentVariation = (s.currentVariation);
  numberOfVariations = (s.numberOfVariations);
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  return *this;
}

void MultiGuiElement::draw() const {
  images[currentFrame + (currentVariation * numberOfFrames)]->draw(
        (getX() + Viewport::getInstance().getX()),
        (getY() + Viewport::getInstance().getY()), getScale());
}

void MultiGuiElement::update(Uint32 ticks) {
  advanceFrame(ticks);
}
