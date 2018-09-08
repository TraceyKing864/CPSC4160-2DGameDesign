#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include "sound.h"
#include "gamedata.h"

SDLSound& SDLSound::getInstance() {
  static SDLSound sdlSound;
  return sdlSound;
}

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  for (unsigned int i = 0; i < soundEffects.size(); ++i) {
    Mix_FreeChunk(soundEffects[i]);
  }
  for (unsigned int i = 0; i < backgroundMusic.size(); ++i) {
    Mix_FreeMusic(backgroundMusic[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() :
  volume(SDL_MIX_MAXVOLUME/10),
  currentSoundEffect(-1),
  currentBackgroundMusic(-1),
  audioRate(22050),
  audioChannels(2),
  audioBuffers(4096),
  backgroundMusic(NULL),
  soundEffects(),
  channels()
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels,
                   audioBuffers)){
    throw std::string("Unable to open audio!");
  }
  std::string soundFile;
  int backgroundMusicCount = Gamedata::getInstance().getXmlInt("sound/backgroundTracks");
  for(int i = 0; i < backgroundMusicCount; i++){
    soundFile.clear();
    std::stringstream ss;
    ss << "sound/background_" << i;
    soundFile = Gamedata::getInstance().getXmlStr(ss.str());
    backgroundMusic.push_back(Mix_LoadMUS(soundFile.c_str()));
    if (!backgroundMusic[i]) throw std::string("Couldn't load " + soundFile)+Mix_GetError();
  }
  
  int soundEffectCount = Gamedata::getInstance().getXmlInt("sound/soundEffects");
  for(int i = 0; i < soundEffectCount; i++){
    soundFile.clear();
    std::stringstream ss;
    ss << "sound/soundEffect_" << i;
    soundFile = Gamedata::getInstance().getXmlStr(ss.str());
    soundEffects.push_back(Mix_LoadWAV(soundFile.c_str()));
  }
  currentBackgroundMusic = 0;
  startMusic();
  
  for (unsigned int i = 0; i < soundEffects.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) {
    Mix_ResumeMusic();
  }
  else {
    Mix_PauseMusic();
  }
}

void SDLSound::operator[](int index) {
  if (currentSoundEffect >= 0) Mix_HaltChannel(currentSoundEffect);
  currentSoundEffect = index;
  Mix_VolumeChunk(soundEffects[index], volume);
  channels[index] =
     Mix_PlayChannel(-1, soundEffects[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(backgroundMusic[currentBackgroundMusic], 0);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
}

void SDLSound::update(Uint32 ticks){
  static Uint32 timeSinceLastUpdate = 0;
  timeSinceLastUpdate += ticks;
  if (timeSinceLastUpdate > 5000) {
    if(!Mix_PlayingMusic()){
      currentBackgroundMusic++;
      if(currentBackgroundMusic == backgroundMusic.size())
        currentBackgroundMusic = 0;
      startMusic();
    }
    timeSinceLastUpdate = 0;
  }
}
