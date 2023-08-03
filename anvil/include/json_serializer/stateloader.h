#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "application.h"

namespace anvil {

class BaseGameObject;
using json = nlohmann::json;

class StateLoader
{
public:
    StateLoader(StateLoader const&) = delete;
    void operator=(StateLoader const&) = delete;

    static StateLoader& instance();
    static std::vector<std::string> loadTextures(const std::string& stateId);
    static std::unique_ptr<BaseGameObject> loadGameObjects(const std::string& stateId);
    static void clearTextures(const std::string& stateId);
    static std::filesystem::path getConfigFile();

private:
    StateLoader() {}
};

}
