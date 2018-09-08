#ifndef multiGuiElement_h
#define multiGuiElement_h

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"

class MultiGuiElement : public Drawable {
public:
  MultiGuiElement(const std::string& name, int nX, int nY);
  MultiGuiElement(const MultiGuiElement&);

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

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned currentVariation;
  unsigned numberOfVariations;
  unsigned frameInterval;
  float timeSinceLastFrame;
  
  void advanceFrame(Uint32 ticks);
  MultiGuiElement& operator=(const MultiGuiElement&);
  
};

#endif
