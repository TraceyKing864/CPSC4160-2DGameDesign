#ifndef projectile_h
#define projectile_h

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"

enum projectile_direction{
  proj_up = 0,
  proj_down = 1,
  proj_left = 2,
  proj_right = 3,
  diag_up_left = 4,
  diag_down_right = 5,
  diag_down_left = 6,
  diag_up_right = 7
};

class Projectile : public Drawable {
public:
  Projectile(const std::string& name, int nX, int nY, int destX, int destY);
  Projectile(const Projectile&);

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
  void resetAnimation();
  bool isFinished(){ return finished;}

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned currentVariation;
  unsigned numberOfVariations;
  unsigned frameInterval;
  float timeSinceLastFrame;
  bool finished;
  Vector2f dest;
  
  void advanceFrame(Uint32 ticks);
  Projectile& operator=(const Projectile&);
};

#endif
