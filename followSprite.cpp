#include "followSprite.h"
#include "gamedata.h"
#include "renderContext.h"

void FollowSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

FollowSprite::FollowSprite( const std::string& name, Drawable& followMe) :
  Drawable(name, Vector2f(0.0, 0.0), Vector2f(0.0, 0.0)),
  images( RenderContext::getInstance()->getImages(name) ),
  following(&followMe),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  currentVariation(0),
  numberOfVariations( Gamedata::getInstance().getXmlInt(name+"/variations") ),
  frameInterval( Gamedata::getInstance().checkTag(name+"/frameInterval") ?
        Gamedata::getInstance().getXmlInt(name+"/frameInterval") :
        Gamedata::getInstance().getXmlInt("sprite_default/frameInterval")),
  timeSinceLastFrame( 0.0 )
{
  // added this code to scale objects
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
}

FollowSprite::FollowSprite(const FollowSprite& s) :
  Drawable(s),
  images(s.images),
  following(s.following),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  currentVariation(s.currentVariation),
  numberOfVariations(s.numberOfVariations),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame )
  { }

FollowSprite& FollowSprite::operator=(const FollowSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  following = (s.following);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  return *this;
}

void FollowSprite::draw() const {
  if(getName() == "shadow")
    images[currentFrame + (numberOfFrames * currentVariation)]->draw(following->getX(), following->getY()+2, getScale());
  else
    images[currentFrame + (numberOfFrames * currentVariation)]->draw(following->getX(), following->getY(), getScale());
}

void FollowSprite::update(Uint32 ticks) {
  advanceFrame(ticks);
}
