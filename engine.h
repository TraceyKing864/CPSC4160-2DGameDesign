#include <vector>
#include <SDL2/SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "viewport.h"
#include "combatScene.h"
#include "sound.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  
private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;
  SDLSound& sound;

  SDL_Renderer * const renderer;
  Viewport& viewport;
  Scene* scene;
  
  bool running;
  bool makeVideo;

  void draw() const;
  void update(Uint32);
  void input();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};
