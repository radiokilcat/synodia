#include "json_serializer/stateloader.h"
#include <iostream>
#include "utils.h"
#include "texturemanager.h"
#include "audio_manager.h"
#include "renderer.h"
#include "game_objects/GameObjectsFactory.h"
#include "game_objects/gameobject.h"
#include <regex>

namespace anvil {

anvil::StateLoader& anvil::StateLoader::instance() {
    static StateLoader instance;
    return instance;
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

std::unique_ptr<BaseGameObject> StateLoader::loadGameObjects(const std::string& stateId)
{
    auto resPath = StateLoader::getConfigFile();
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

    auto objects = data[stateId]["objects"];

    // for now GameScene can be the only one
    auto scene = objects["GameScene"];

    auto sceneObject = GameObjectFactory::instance().createGameObject(scene.value("id", std::string("GameScene")));
    for (const auto& [child, params] : scene["childs"].items())
    {
        loadChild(sceneObject.get(), child, params);
    }
    return std::move(sceneObject);
}

void StateLoader::loadChild(BaseGameObject* parent, std::string id, json item) {
    if (item.is_array()) {
        for (auto& childParams : item)
        {
            auto childObj = GameObjectFactory::instance().createGameObject(id);
            childObj->from_json(childParams);
            childObj->init();
            parent->addChildObject(std::move(childObj));
        }
    }
    else if (item.is_object())
    {
        auto childObj = GameObjectFactory::instance().createGameObject(id);
        childObj->from_json(item);
        childObj->init();
        parent->addChildObject(std::move(childObj));
    }
    else if (item.is_string())
    {
        std::smatch sm;
        const std::string command = item;
        if (regex_match(command, sm, std::regex{ "\@(?:include)[(](.+)[)]" })) {
            const auto path = sm[1].str();
            auto childObj = includeObject(path);
            parent->addChildObject(std::move(childObj));
        }
    }
}

std::unique_ptr<BaseGameObject> StateLoader::includeObject(const std::string path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return {};
    }
    nlohmann::json data = nlohmann::json::parse(file);

    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON" << std::endl;
        return {};
    }
    auto object = GameObjectFactory::instance().createGameObject(data["id"]);
    object->from_json(data);
    object->init();
    for (const auto& [child, params] : data["childs"].items())
    {
        loadChild(object.get(), child, params);
    }
    return object;
}

std::vector<std::string> anvil::StateLoader::loadTextures(const std::string& stateId)
{
    auto resPath = StateLoader::getConfigFile();

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
    auto resPath = StateLoader::getConfigFile();

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

