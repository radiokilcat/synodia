#include "audio_manager.h"

#include <iostream>

namespace anvil {

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::initAudio()
{
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        std::exit(1);
    }
}

bool AudioManager::loadFile(const std::string& fileName, const std::string& id)
{
    Mix_Music* sound = Mix_LoadMUS(fileName.c_str());
    if (sound != 0)
    {
        sounds_[id] = sound;
        return true;
    }
    std::cout << Mix_GetError() << " : " << fileName.c_str() << std::endl;
    return false;

}

//ToDo: Add PlayChannel support for playing music and sounds separately
void AudioManager::playMusic(const std::string& id, bool loop)
{
    if( Mix_PlayingMusic() == 0 )
    {
        Mix_PlayMusic(sounds_[id], -1);
    }
}

void AudioManager::pauseMusic()
{
    if( Mix_PlayingMusic() )
    {
        Mix_PauseMusic();
    }
}

void AudioManager::resumeMusic()
{
    if( Mix_PausedMusic() )
    {
        Mix_ResumeMusic();
    }

}

void AudioManager::stopMusic()
{
    Mix_HaltMusic();
}

void AudioManager::cleanup()
{
    Mix_HaltChannel(-1);
    for (auto sound: sounds_)
    {
        Mix_FreeMusic(sound.second);
    }
    Mix_CloseAudio();
}

AudioManager::AudioManager()
{

}

}
