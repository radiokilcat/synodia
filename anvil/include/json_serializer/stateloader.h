#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "application.h"

namespace anvil {

class IGameObject;
using json = nlohmann::json;

class StateLoader
{
public:
    StateLoader(StateLoader const&) = delete;
    void operator=(StateLoader const&) = delete;

    static StateLoader& instance();
    static std::filesystem::path getStateConfigFile(const std::string& stateId);
    static std::vector<std::string> loadTextures(const std::string& stateId);
    static void loadAudio(const std::string& stateId);
    static std::shared_ptr<IGameObject> loadGameObjects(const std::string& stateId);
    static std::shared_ptr<IGameObject> loadMenu(const std::string& stateId);
    static nlohmann::json loadObjectTemplate(std::shared_ptr<IGameObject>& object, const nlohmann::json& data);
    static std::shared_ptr<IGameObject> loadObjectAndChildren(const json& rootData);
    static void clearTextures(const std::string& stateId);
    static std::filesystem::path getConfigFile();

private:
    StateLoader() {}
};

}
