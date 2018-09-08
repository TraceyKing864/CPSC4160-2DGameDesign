#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() {
  if(scene)
    delete scene;

  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  sound( SDLSound::getInstance() ),
  renderer( rc->getRenderer() ),
  viewport( Viewport::getInstance() ),
  scene(new CombatScene("scene")),
  running(false),
  makeVideo(false)
{
  //seed the randomizer
  srand(time(NULL));
  
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  SDL_RenderClear(renderer);
  if(scene)
    scene->draw();
  viewport.draw();
  viewport.scale();
  
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  if(scene){
    scene->update(ticks);
    if(scene->resetMe()){
      delete scene;
      scene = new CombatScene("scene");
    }
  }
  sound.update(ticks);
  viewport.update(ticks); // always update viewport last
}

void Engine::input() {
  SDL_Event event;
  const Uint8* keystate;
  // The next loop polls for events, guarding against key bounce:
  while ( SDL_PollEvent(&event) ) {
    keystate = SDL_GetKeyboardState(NULL);
    if (event.type ==  SDL_QUIT) { running = false; break; }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
        running = false;
        break;
      }
      else if ( keystate[SDL_SCANCODE_T] ) {
        scene->input(SDL_SCANCODE_T);
      }
      else if ( keystate[SDL_SCANCODE_W] ) {
        scene->input(SDL_SCANCODE_W);
      }
      else if ( keystate[SDL_SCANCODE_A] ) {
        scene->input(SDL_SCANCODE_A);
      }
      else if ( keystate[SDL_SCANCODE_S] ) {
        scene->input(SDL_SCANCODE_S);
      }
      else if ( keystate[SDL_SCANCODE_D] ) {
        scene->input(SDL_SCANCODE_D);
      }
      else if ( keystate[SDL_SCANCODE_J] ) {
        scene->input(SDL_SCANCODE_J);
      }else if ( keystate[SDL_SCANCODE_K] ) {
        scene->input(SDL_SCANCODE_K);
      }else if ( keystate[SDL_SCANCODE_I] ) {
        scene->input(SDL_SCANCODE_I);
      }else if ( keystate[SDL_SCANCODE_R] ) {
        scene->input(SDL_SCANCODE_R);
      }
      else if ( keystate[SDL_SCANCODE_M] ) {
        scene->input(SDL_SCANCODE_M);
      }
      else if ( keystate[SDL_SCANCODE_F1] || keystate[SDL_SCANCODE_U]) {
        viewport.toggleHelp();
      }
      else if ( keystate[SDL_SCANCODE_F2] || keystate[SDL_SCANCODE_P]) {
        viewport.toggleFps();
      }
      if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
        std::cout << "Initiating frame capture" << std::endl;
        makeVideo = true;
      }
      else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
        std::cout << "Terminating frame capture" << std::endl;
        makeVideo = false;
      }
    }
  }

    // In this section of the event loop we allow key bounce:
  
}

void Engine::play() {
  running = true;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while (running) {
    input();
    ticks = clock.getElapsedTicks();
    update(ticks);
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
    
  }
}
