#include "projectile.h"

#include "gamedata.h"
#include "renderContext.h"

Projectile::Projectile( const std::string& name, int nX, int nY, int destX, int destY) :
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
  finished(false), dest(Vector2f(destX, destY))
{
  if(Gamedata::getInstance().checkTag(name+"/scale/s"))
    setScale(Gamedata::getInstance().getXmlInt(name+"/scale/s"));
  else
    setScale(Gamedata::getInstance().getXmlInt("sprite_default/scale/s"));
  
  float eta = Gamedata::getInstance().getXmlInt(name+"/eta");
  float velocityX;
  if(nX > destX)
    velocityX = -(abs((float)getX() - (float)destX) / eta);
  else if(nX < destX)
    velocityX = (abs((float)getX() - (float)destX) / eta);
  else
    velocityX = 0;
  float velocityY;
  if(nY > destY)
    velocityY = -(abs((float)getY() - (float)destY) / eta);
  else if(nY < destY)
    velocityY = (abs((float)getY() - (float)destY) / eta);
  else
    velocityY = 0;
  
  if(nY > destY && nX > destX)
    setVariation(projectile_direction::diag_up_left);
  else if(nY < destY && nX < destX)
    setVariation(projectile_direction::diag_down_right);
  else if(nY < destY && nX > destX)
    setVariation(projectile_direction::diag_down_left);
  else if(nY > destY && nX < destX)
    setVariation(projectile_direction::diag_up_right);
  
  else if(nY > destY)
    setVariation(projectile_direction::proj_up);
  else if(nY < destY)
    setVariation(projectile_direction::proj_down);
  else if(nX > destX)
    setVariation(projectile_direction::proj_left);
  else if(nX < destX)
    setVariation(projectile_direction::proj_right);

  setVelocity(Vector2f(velocityX, velocityY));
    
}

Projectile::Projectile(const Projectile& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  currentVariation(s.currentVariation),
  numberOfVariations(s.numberOfVariations),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  finished( s.finished ),
  dest( s.dest )
  { }

Projectile& Projectile::operator=(const Projectile& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  currentVariation = (s.currentVariation);
  numberOfVariations = (s.numberOfVariations);
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  finished = ( s.finished );
  dest = ( s.dest );
  return *this;
}

void Projectile::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    setPosition(getPosition() + getVelocity());
    
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
    if(getX() == dest[0] && getY() == dest[1]){
      finished = true;
      setVelocity(Vector2f(0, 0));
    }
  }
}

void Projectile::resetAnimation(){
  finished = false;
  currentFrame = 0;
}
void Projectile::draw() const {
  if(!finished){
    images[currentFrame + (currentVariation * numberOfFrames)]->draw(getX(), getY(), getScale());
  }
}

void Projectile::update(Uint32 ticks) {
  if(!finished){
    advanceFrame(ticks);
  }
}
