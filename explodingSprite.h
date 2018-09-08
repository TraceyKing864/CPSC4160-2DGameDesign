#ifndef explodingSprite_h
#define explodingSprite_h

#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <list>
#include "drawable.h"
#include "chunk.h"

class ExplodingSprite : public Drawable {
public:
  ExplodingSprite(Image* nImage, int nX, int nY);
  ExplodingSprite(const ExplodingSprite&);
  ~ExplodingSprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return image;
  }
  virtual int getScaledWidth()  const {
    return getScale()*image->getWidth();
  }
  virtual int getScaledHeight()  const {
    return getScale()*image->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return image->getSurface();
  }
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool getFinished(){return chunks.empty();}

protected:
  std::list<Chunk*> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk*> freeList; // When a chunk gets out of range it goes here
  Image* image;

  ExplodingSprite& operator=(const ExplodingSprite&);
};

#endif
