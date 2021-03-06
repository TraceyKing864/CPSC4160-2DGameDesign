#include "multiSprite.h"
#include "gamedata.h"
#include "renderContext.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}
void MultiSprite::advanceMove(Uint32 ticks) {
  timeSinceLastMove += ticks;
  if (timeSinceLastMove > moveInterval) {
    setPosition(getPosition() + getVelocity());
    timeSinceLastMove = 0;
    
    Vector2f temp = getPosition();
    if (((static_cast<int>(temp[0]) % 16) == 0) && ((static_cast<int>(temp[1]) % 16) == 0))
      setVelocity(Vector2f(0.0, 0.0));
  }
}

MultiSprite::MultiSprite( const std::string& name, int nX, int nY) :
  Drawable(name, Vector2f(nX, nY), Vector2f(0.0, 0.0)),
  images( RenderContext::getInstance()->getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  currentVariation(0),
  numberOfVariations( Gamedata::getInstance().getXmlInt(name+"/variations") ),
  frameInterval( Gamedata::getInstance().checkTag(name+"/frameInterval") ?
        Gamedata::getInstance().getXmlInt(name+"/frameInterval") :
        Gamedata::getInstance().getXmlInt("sprite_default/frameInterval")),
  timeSinceLastFrame( 0.0 ),
  moveInterval(Gamedata::getInstance().checkTag(name+"/moveInterval") ?
        Gamedata::getInstance().getXmlInt(name+"/moveInterval") :
        Gamedata::getInstance().getXmlInt("sprite_default/moveInterval")),
  timeSinceLastMove(0.0)
{
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
  
}

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  currentVariation(s.currentVariation),
  numberOfVariations(s.numberOfVariations),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame )
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  return *this;
}

void MultiSprite::draw() const {
  images[currentFrame + (currentVariation * numberOfFrames)]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);
  advanceMove(ticks);
}
