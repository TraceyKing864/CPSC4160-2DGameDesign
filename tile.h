#ifndef TILE__H
#define TILE__H

#include <iostream>
#include "staticSprite.h"

class Tile{
private:
  std::string name;   //tile_ID/name
  StaticSprite* sprite;
  int type;           //z-order location on sprite sheet
  //Note: int data type used for collision to extend into different types of collision
  //  0 is not passable
  int collision;
public:
  //This enum is used for readability in choosing Tile::type
  enum TILE_TYPE{
    top_left = 0,
    up = 1,
    top_right = 2,
    left = 3,
    middle = 4,
    right = 5,
    bot_left = 6,
    bot = 7,
    bot_right = 8
  };

  Tile(std::string nName, int nType, int locX, int locY);
  Tile(const Tile& other);
  ~Tile();
  Tile& operator=(const Tile& other);
  void draw() const;
  void update(Uint32 ticks);
  
  int getCollision(){ return collision; };
  void setType(int nType);
  
};
#endif
