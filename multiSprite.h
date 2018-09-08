#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"

enum direction{
  down = 0,
  left = 1,
  right = 2,
  up = 3
};

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string& name, int nX, int nY);
  MultiSprite(const MultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  Image* getImage() {
    return images[currentFrame];
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  void setDirection(direction d){ currentVariation = d; }
  void setVariation(int i){ currentVariation = i;}

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned currentVariation;
  unsigned numberOfVariations;
  unsigned frameInterval;
  float timeSinceLastFrame;
  unsigned moveInterval;
  float timeSinceLastMove;

  void advanceFrame(Uint32 ticks);
  void advanceMove(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
  
  Vector2f makeVelocity(int, int) const;
  Vector2f makeVelocity(Vector2f) const;
};
#endif
