#include "animation.h"
#include "gamedata.h"
#include "renderContext.h"

Animation::Animation( const std::string& name, int nX, int nY) :
  Drawable(name, Vector2f(nX, nY), Vector2f(0.0, 0.0)),
  images( RenderContext::getInstance()->getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  currentVariation(0),
  numberOfVariations( Gamedata::getInstance().getXmlInt(name+"/variations") ),
  frameInterval( Gamedata::getInstance().checkTag(name+"/frameInterval") ?
        Gamedata::getInstance().getXmlInt(name+"/frameInterval") :
        Gamedata::getInstance().getXmlInt("sprite_default/frameInterval")),
  timeSinceLastFrame( 0.0 ), finished(false),
  cycles( Gamedata::getInstance().getXmlInt(name+"/cycles"))
{
  // added this code to scale objects
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
}

Animation::Animation(const Animation& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  currentVariation(s.currentVariation),
  numberOfVariations(s.numberOfVariations),
  finished(s.finished),
  cycles(s.cycles)
  { }

Animation& Animation::operator=(const Animation& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  finished = (s.finished );
  cycles = (s.cycles );
  return *this;
}
void Animation::setVariation(int i){
  currentVariation = i;
}
void Animation::resetAnimation(){
  finished = false;
  currentFrame = 0;
  cycles = Gamedata::getInstance().getXmlInt(getName()+"/cycles");
}

void Animation::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    if(currentFrame == numberOfFrames-1)  //means a complete cycle completed
      cycles--;
    if(cycles == 0)
      finished = true;
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

void Animation::draw() const {
  if(!finished){
    images[currentFrame + (currentVariation * numberOfFrames)]->draw(getX(), getY(), getScale());
  }
}

void Animation::update(Uint32 ticks) {
  if(!finished)
    advanceFrame(ticks);
}
