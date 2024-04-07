#include "json_serializer/stateloader.h"
#include <iostream>
#include "utils.h"
#include "texturemanager.h"
#include "audio_manager.h"
#include "game_objects/GameObjectsFactory.h"
#include "game_objects/gameobject.h"
#include "AnvilImgui/ImguiSystem.h"
#include <regex>
#include <fmt/format.h>

namespace anvil {

StateLoader& StateLoader::instance() {
    static StateLoader instance;
    return instance;
}

std::filesystem::path StateLoader::getStateConfigFile(const std::string& stateId)
{
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "res" / fmt::format("{}.json", stateId);

    if (!std::filesystem::exists(resPath)) {
        std::cerr << "File" << resPath << " doesn't exist" << std::endl;
        return {};
    }

    if (std::filesystem::file_size(resPath) == 0) {
        std::cerr << "File" << resPath << " is empty" << std::endl;
        return {};
    }
    return resPath;
}

std::filesystem::path StateLoader::getConfigFile()
{
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "res" / "world.json";

    if (!std::filesystem::exists(resPath)) {
        std::cerr << "File" << resPath << " doesn't exist" << std::endl;
        return {};
    }

    if (std::filesystem::file_size(resPath) == 0) {
        std::cerr << "File" << resPath << " is empty" << std::endl;
        return {};
    }
    return resPath;
}

std::unique_ptr<IGameObject> StateLoader::loadGameObjects(const std::string& stateId)
{
    auto resPath = getStateConfigFile(stateId);
    std::ifstream file(resPath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return {};
    }
    nlohmann::json data = nlohmann::json::parse(file);

    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON" << std::endl;
        return {};
    }

    auto scene = loadObject(data.at("GameScene"));
    return std::move(std::unique_ptr<IGameObject>(scene));
}

void StateLoader::loadObjectTemplate(IGameObject* object, const nlohmann::json& data) {
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "res" / fmt::format("objects_templates.json");
    
    std::ifstream file(resPath);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }
    
    nlohmann::json templateData = nlohmann::json::parse(file);

    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON" << std::endl;
    }

    auto defaultVal = templateData.at(data.at("type"));
    for (auto& [key, val] : data.items()) {
        defaultVal[key] = val;
    }
    object->from_json(defaultVal);
}
    
IGameObject* StateLoader::loadObject(const json& item) {
    auto parent = GameObjectFactory::instance().createGameObject(item.at("type")).release();
    loadObjectTemplate(parent, item);
    parent->init();
    
    if (item.find("childs") != item.end() && item.at("childs").is_array()) {
        for (auto& childJson : item.at("childs")) {
            auto childNode = GameObjectFactory::instance().createGameObject(childJson.at("type")).release();
            parent->addChild(std::unique_ptr<IGameObject>(loadObject(childJson)));
        }
    }
    return parent;
}

std::vector<std::string> StateLoader::loadTextures(const std::string& stateId)
{
    auto resPath = getConfigFile();

    std::cout << "assets path: " <<  resPath << std::endl;
    std::ifstream file(resPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return {};
    }
    nlohmann::json data = nlohmann::json::parse(file);

    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON" << std::endl;
        return {};
    }

    auto textures = data[stateId]["textures"];
    std::vector<std::string> textureIds;
    for (const auto& [name, path] : textures.items()) {
        std::filesystem::path fullPath = std::filesystem::canonical(std::filesystem::current_path() / "res" / path.get<std::string>());
        if (TextureManager::instance()->loadTexture(fullPath.string(), name,
                                                    Application::Instance()->getRenderer()->getRenderer()))
        {
            textureIds.push_back(name);
        }

    }
    return textureIds;
}

void StateLoader::loadAudio(const std::string& stateId)
{
    auto resPath = getConfigFile();

    std::ifstream file(resPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    nlohmann::json data = nlohmann::json::parse(file);

    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON" << std::endl;
        return;
    }

    auto sounds = data[stateId]["audio"];
    for (const auto& [name, path] : sounds.items()) {
        auto fullPath = std::filesystem::canonical(std::filesystem::current_path() / "res" / path.get<std::string>());
        AudioManager::instance().loadFile(fullPath.string(), name);
    }
}

}

