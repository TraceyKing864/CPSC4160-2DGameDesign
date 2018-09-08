#include <cmath>
#include "chunk.h"

Chunk::Chunk( const Vector2f& pos, const Vector2f vel, const string& name, Image* fm) :
    Drawable(name, pos, vel),
    distance(0),
    maxDistance(Gamedata::getInstance().getXmlInt("sprite_default/distance")),
    tooFar(false),
    image(fm)
{ }

void Chunk::update(Uint32 ticks) {
  float yincr = getVelocityY() * static_cast<float>(ticks) * 0.001;
  setY( getY()- yincr );
  float xincr = getVelocityX() * static_cast<float>(ticks) * 0.001;
  setX( getX()- xincr );
  distance += ( hypot(xincr, yincr) );
  if (distance > maxDistance) tooFar = true;
}

void Chunk::draw() const {
  image->draw(getX(), getY(), getScale());
}

