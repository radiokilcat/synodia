#include "audio_manager.h"

#include <iostream>

namespace anvil {

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::initAudio()
{
    if (!MIX_Init()) {
        printf("SDL_mixer could not initialize! Error: %s\n", SDL_GetError());
        return false;
    }
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer_) {
        printf("Failed to create mixer device! Error: %s\n", SDL_GetError());
        return false;
    }
    musicTrack_ = MIX_CreateTrack(mixer_);
    if (!musicTrack_) {
        printf("Failed to create music track! Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool AudioManager::loadFile(const std::string& fileName, const std::string& id)
{
    if (!mixer_) return false;
    MIX_Audio* audio = MIX_LoadAudio(mixer_, fileName.c_str(), true);
    if (!audio) {
        std::cout << SDL_GetError() << " : " << fileName << std::endl;
        return false;
    }
    sounds_[id] = audio;
    return true;
}

void AudioManager::playMusic(const std::string& id, bool loop)
{
    if (!musicTrack_ || sounds_.find(id) == sounds_.end()) return;
    MIX_SetTrackAudio(musicTrack_, sounds_[id]);
    MIX_PlayTrack(musicTrack_, 0);
}

void AudioManager::pauseMusic()
{
    if (musicTrack_) MIX_PauseTrack(musicTrack_);
}

void AudioManager::resumeMusic()
{
    if (musicTrack_) MIX_ResumeTrack(musicTrack_);
}

bool AudioManager::isSoundMuted()
{
    return mute_;
}

void AudioManager::stopMusic()
{
    if (musicTrack_) MIX_StopTrack(musicTrack_, 0);
}

void AudioManager::cleanup()
{
    stopMusic();
    if (musicTrack_) {
        MIX_DestroyTrack(musicTrack_);
        musicTrack_ = nullptr;
    }
    for (auto& [id, audio] : sounds_) {
        MIX_DestroyAudio(audio);
    }
    sounds_.clear();
    if (mixer_) {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
}

AudioManager::AudioManager() {}

AudioManager::~AudioManager() { cleanup(); }

}
