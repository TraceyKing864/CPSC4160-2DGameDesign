#ifndef cursor_h
#define cursor_h

#include <iostream>
#include <string>

#include "multiSprite.h"


class Cursor{
private:
  std::string name;
  MultiSprite* sprite;

  int x;  //this refers to x and y location on the board
  int y;
  int animating;  //used to block input/animate while moving
  bool shown;
  
public:
  Cursor(std::string nName, int nX, int nY);
  Cursor(const Cursor& other);
  ~Cursor();
  Cursor& operator=(const Cursor& other);
  //common game object functions
  void draw() const;
  void update(Uint32 ticks);
  //getters & setters
  int getX(){return x;}
  int getY(){return y;}
  void setX(int nX);
  void setY(int nY);
  MultiSprite* getSprite(){return sprite;}
  int getAnimating(){return animating;}
  void show(){shown = true;}
  void hide(){shown = false;}
  
  //commands  
  void move(direction d);

};



#endif
