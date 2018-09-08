#ifndef STATICSPRITE__H
#define STATICSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"

class StaticSprite : public Drawable {
public:
  StaticSprite(const std::string& name, int nX, int nY);
  StaticSprite(const std::string& name, int nX, int nY, int nType);
  StaticSprite(const StaticSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[type];
  }
  int getScaledWidth()  const { 
    return getScale()*images[type]->getWidth();
  } 
  int getScaledHeight()  const { 
    return getScale()*images[type]->getHeight();
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[type]->getSurface();
  }
  void setType(int nType) {
    type = nType;
  }

protected:
  std::vector<Image *> images;

  unsigned type;
  unsigned rows;
  unsigned cols;

  StaticSprite& operator=(const StaticSprite&);
};
//
#endif
