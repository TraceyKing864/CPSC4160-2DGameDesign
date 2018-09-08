#ifndef chunk_h
#define chunk_h

#include <iostream>
#include "drawable.h"
#include "gamedata.h"

class Chunk : public Drawable {
public:
  Chunk( const Vector2f& pos, const Vector2f vel, const string& name, Image* fm);

  Chunk(const Chunk& )=default;
  Chunk(      Chunk&&)=default;
  Chunk& operator=(const Chunk& )=default;
  Chunk& operator=(      Chunk&&)=default;

  ~Chunk(){
    delete image;
  }
  virtual void update(Uint32 ticks);
  virtual void draw() const;
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
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
private:
  float distance;
  float maxDistance;
  bool tooFar;
  Image* image;
};


#endif
