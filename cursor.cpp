#include "cursor.h"
#include "gamedata.h"


Cursor::Cursor(std::string nName, int nX, int nY) :
      name(nName),
      sprite(new MultiSprite("cursor", (nX * 16), (nY * 16))),
      x(nX),
      y(nY),
      animating(0),
      shown(true)
{}
Cursor::Cursor(const Cursor& other) :
      name(other.name),
      sprite(other.sprite),
      x(other.x),
      y(other.y),
      animating(other.animating),
      shown(other.shown)
{}
Cursor::~Cursor(){
  delete sprite;
}
Cursor& Cursor::operator=(const Cursor& other){
  name = other.name;
  sprite = other.sprite;
  x = other.x;
  y = other.y;
  animating = other.animating;
  shown = other.shown;
  return *this;
}

void Cursor::draw() const{
  if(shown)
    sprite->draw();
}
void Cursor::update(Uint32 ticks){
  if(animating){
    if(animating == 1){
      if((sprite->getVelocityX() == 0) && (sprite->getVelocityY() == 0)) //if 0 then done animating
        animating = 0;
    }
    //other animations here ie (animating == 2){}
  }
  sprite->update(ticks);
}

void Cursor::setX(int nX){
  x = nX;
  sprite->setX(x * 16);
}
void Cursor::setY(int nY){
  y = nY;
  sprite->setY(y * 16);
}

void Cursor::move(direction d){
  if(animating > 0) return;
  if(d == direction::up){
    y--;
    sprite->setVelocityY(-2.0);
  }else if(d == direction::down){
    y++;
    sprite->setVelocityY(2.0);
  }else if(d == direction::left){
    x--;
    sprite->setVelocityX(-2.0);
  }else if(d == direction::right){
    x++;
    sprite->setVelocityX(2.0);
  }
  if(Gamedata::getInstance().getXmlBool("coutmovement"))
    std::cout << name << " moved to: (" << x << ", " << y << ") (z-" << (y * 27 + x) << ") " << std::endl;
  animating = 1;
}
