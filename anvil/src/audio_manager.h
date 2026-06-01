#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <map>

namespace anvil {

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();
    static AudioManager& instance();

    bool initAudio();

    bool loadFile(const std::string &fileName, const std::string &id);
    void playMusic(const std::string &id, bool loop = false);
    void pauseMusic();
    void resumeMusic();
    bool isSoundMuted();
    void stopMusic();

    void cleanup();

private:
    MIX_Mixer* mixer_ = nullptr;
    MIX_Track* musicTrack_ = nullptr;
    std::map<std::string, MIX_Audio*> sounds_;
    bool mute_ = false;
};

}
