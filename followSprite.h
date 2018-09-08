#ifndef followSprite_h
#define followSprite_h

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"

class FollowSprite : public Drawable {
public:
  FollowSprite(const std::string& name, Drawable& followMe);
  FollowSprite(const FollowSprite&);

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
  void setVariation(int i){ currentVariation = i;}

protected:
  std::vector<Image *> images;
  Drawable* following;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned currentVariation;
  unsigned numberOfVariations;
  unsigned frameInterval;
  float timeSinceLastFrame;

  void advanceFrame(Uint32 ticks);
  FollowSprite& operator=(const FollowSprite&);
  
};

#endif
