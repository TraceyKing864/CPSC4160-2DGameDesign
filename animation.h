#ifndef animation_h
#define animation_h

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"



class Animation : public Drawable {
public:
  Animation(const std::string& name, int nX, int nY);
  Animation(const Animation&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
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
  bool isFinished(){return finished;}
  void resetAnimation();
  void setVariation(int i);

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned currentVariation;
  unsigned numberOfVariations;
  unsigned frameInterval;
  float timeSinceLastFrame;
  bool finished;
  int cycles; //number of animation cycles until it is done

  void advanceFrame(Uint32 ticks);
  Animation& operator=(const Animation&);
  
  Vector2f makeVelocity(int, int) const;
  Vector2f makeVelocity(Vector2f) const;
};
#endif
