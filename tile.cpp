#include "tile.h"
#include "gamedata.h"

Tile::Tile(std::string nName, int nType, int locX, int locY) :
      name(nName), sprite(new StaticSprite(nName, locX, locY)), type(nType),
      collision(Gamedata::getInstance().getXmlInt(name+"/collision"))
{}
Tile::Tile(const Tile& other) : name(other.name), sprite(other.sprite),
      type(other.type), collision(other.collision)
{}
Tile::~Tile(){
  delete sprite;
}
Tile& Tile::operator=(const Tile& other){
  name = other.name;
  sprite = other.sprite;
  type = other.type;
  collision = other.collision;
  return *this;
}
void Tile::draw() const{
  sprite->draw();
}
void Tile::update(Uint32 ticks){
  //Intentionally left empty
}
void Tile::setType(int nType){
  type = nType;
}
