#ifndef sound_h
#define sound_h

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

// In this demo, we use Mix_Music *Mix_LoadMUS(const char *file)
// Where file is the name of the music file to use.
//
// SDL_mixer supports music and sound samples from the following formats:
// - WAVE/RIFF (.wav)
// - AIFF (.aiff)
// - VOC (.voc)
// - MOD (.mod .xm .s3m .669 .it .med and more) using included mikmod
// - MIDI (.mid) using timidity or native midi hardware
// - OggVorbis (.ogg) requiring ogg/vorbis libraries on system
// - MP3 (.mp3) requiring SMPEG library on system
// - also any command-line player, which is not mixed by SDL_mixer...
//   that requires a command to play with.

class SDLSound {
public:
  static SDLSound& getInstance();
  void startMusic();
  void stopMusic();      // stop all sounds
  void toggleMusic();    // toggle music on/off
  void operator[](int);  // play the indexed sound
  void update(Uint32 ticks);
private:
  int volume;
  int currentSoundEffect;
  int currentBackgroundMusic;

  int audioRate;
  int audioChannels;
  int audioBuffers;
  std::vector<Mix_Music*> backgroundMusic;
  std::vector<Mix_Chunk*> soundEffects;
  std::vector<int> channels;
  SDLSound();
  ~SDLSound();
  SDLSound(const SDLSound&);
  SDLSound& operator=(const SDLSound&);
};

#endif
