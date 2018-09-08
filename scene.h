#ifndef SCENE__H
#define SCENE__H

#include <iostream>
#include <string>
#include <SDL2/SDL.h> //for Uint32

class Scene{
private:
  std::string name;
public:
  Scene(std::string nName) : name(nName){}
  virtual ~Scene(){}
  Scene(const Scene& other) : name(other.name){}
  Scene& operator=(const Scene& other){ name = other.name; return *this;}
  
  virtual void input(int scancode) = 0;
  virtual void update(Uint32 ticks) = 0;
  virtual void draw() = 0;
  virtual bool resetMe() = 0;
};
#endif
