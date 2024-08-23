#include "json_serializer/stateloader.h"
#include <iostream>
#include "utils.h"
#include "texturemanager.h"
#include "audio_manager.h"
#include "game_objects/GameObjectsFactory.h"
#include "game_objects/gameobject.h"
#include <regex>
#include <fmt/format.h>

#include "components/Collision2DComponent.h"
#include "components/MovementIsoComponent.h"
#include "components/Sprite2DComponent.h"
#include "components/TextComponent.h"
#include "components/Transform2DComponent.h"

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

std::filesystem::path StateLoader::getConfigFile() {
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

std::shared_ptr<IGameObject> StateLoader::loadGameObjects(const std::string& stateId) {
    if (stateId == "EDIT") {
        return loadGameObjects("play");
    }
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

    return loadObjectAndChildren(data.at("GameScene"));
}

nlohmann::json StateLoader::loadObjectTemplate(std::shared_ptr<IGameObject>& object, const nlohmann::json& data) {
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
    return defaultVal;
}
    
std::shared_ptr<IGameObject> StateLoader::loadObjectAndChildren(const json& rootData) {
    auto root = GameObjectFactory::instance().createGameObject(rootData.at("type"));
    auto newData = loadObjectTemplate(root, rootData);
    if (newData.find("position") != newData.end()) {
        root->addComponent(std::make_shared<Transform2DComponent>(newData["position"]));
    }
    if (newData.find("movementMode") != newData.end()) {
        if (newData["movementMode"] == "isometric") {
            auto movementComp = std::make_shared<MovementIsoComponent>();
            root->addComponent(movementComp);
            movementComp->setOwner(root);
        }
    }
    if (newData.find("image") != newData.end()) {
        nlohmann::json spriteData = {
            { "width", newData.value("width", 0) },
            { "height", newData.value("height", 0) },
            { "image", newData["image"] }
        };
        auto sprite = std::make_shared<Sprite2DComponent>(spriteData);
        root->addComponent(sprite);
        sprite->setOwner(root);
    }
    if (newData.find("sprite") != newData.end()) {
        nlohmann::json spriteData = {
            { "width", newData.value("width", 0) },
            { "height", newData.value("height", 0) },
            { "sprite", newData["sprite"] }
        };
        auto sprite = std::make_shared<Sprite2DComponent>(spriteData);
        root->addComponent(sprite);
        sprite->setOwner(root);
    }
    if (newData.find("text") != newData.end()) {
        nlohmann::json textData = {
            { "width", newData.value("width", 0) },
            { "height", newData.value("height", 0) },
            { "text", newData.value("text", "") },
            { "color", newData["textColor"] }
        };
        root->addComponent(std::make_shared<TextComponent>(textData));
    }
    if (newData.find("CollisionComponent") != newData.end()) {
        auto centerX = 0;
        auto centerY = 0;
        auto collider = std::make_shared<CollisionComponent>(newData["CollisionComponent"].value("width", 10), newData["CollisionComponent"].value("height", 10));
        root->addComponent(collider);
        collider->setOwner(root);
    }
    if (newData.find("childs") != newData.end() && newData.at("childs").is_array()) {
        for (auto& childJson : newData.at("childs")) {
            root->addChild(loadObjectAndChildren(childJson));
        }
    }
    root->from_json(newData);
    return root;
}

std::vector<std::string> StateLoader::loadTextures(const std::string& stateId) {
    if (stateId == "EDIT") {
        return loadTextures("play");
    }
    auto resPath = getStateConfigFile(stateId);

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

    auto textures = data["textures"];
    std::vector<std::string> textureIds;
    for (const auto& [name, path] : textures.items()) {
        std::filesystem::path fullPath = std::filesystem::canonical(std::filesystem::current_path() / "res" / path.get<std::string>());
        if (TextureManager::instance()->loadTexture(fullPath.string(), name,
                Application::Instance()->getRenderer()->getRenderer()))
        {
            textureIds.push_back(name);
        }
        else {
            std::cerr << "Failed to load texture " << name <<  std::endl;
        }

    }
    return textureIds;
}

void StateLoader::loadAudio(const std::string& stateId) {
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

