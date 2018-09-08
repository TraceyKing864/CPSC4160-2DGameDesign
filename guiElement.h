#ifndef guiElement_hpp
#define guiElement_hpp

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "drawable.h"

class GuiElement : public Drawable {
public:
  GuiElement(const std::string& name, int nX, int nY);
  GuiElement(const std::string& name, int nX, int nY, int nType);
  GuiElement(const GuiElement&);

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

  GuiElement& operator=(const GuiElement&);
};


#endif
