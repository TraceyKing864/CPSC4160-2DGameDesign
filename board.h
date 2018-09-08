#ifndef BOARD__H
#define BOARD__H

#include <iostream>
#include <vector>
#include "tile.h"
#include "viewport.h"


class Board{
private:
  unsigned int width;
  unsigned int height;
  std::vector<Tile*> tiles;
  
  float viewX;
  float viewY;
  const Viewport& view;
public:
  Board(std::string fileName, int nWidth, int nHeight);
  Board(const Board& Board);
  ~Board();
  Board& operator=(const Board& other);
  void draw();
  void update(Uint32 ticks);

  int getCollision(int z);
  int getCollision(int x, int y);
  int getWidth(){return width;}
  int getHeight(){return height;}
};
#endif
