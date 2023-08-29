#pragma once

#include <SDL_mixer.h>

#include <string>
#include <map>

namespace anvil {

class AudioManager
{
public:
    AudioManager();
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
    std::map<std::string, Mix_Music*> sounds_;
    bool mute_;
};

}
