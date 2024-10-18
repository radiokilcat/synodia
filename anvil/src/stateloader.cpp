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

std::filesystem::path StateLoader::getTexturesConfigFile()
{
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets" / "objects" / "textures.json";

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
    
std::filesystem::path StateLoader::getStateConfigFile(const std::string& stateId)
{
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets" / "objects" / fmt::format("{}.json", stateId);

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


std::filesystem::path StateLoader::getAudioConfigFile() {
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets" / "objects" / "audio.json";

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

    auto scene = loadObject(data.at("GameScene"));
    loadUI(scene);
    return scene;
    // return loadObjectAndChildren(data.at("GameScene"));
}

    nlohmann::json StateLoader::loadObjectTemplate(std::shared_ptr<IGameObject>& object, const nlohmann::json& data) {
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets" / "objects" / fmt::format("objects_templates.json");

    std::ifstream file(resPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open template file" << std::endl;
        return data;
    }

    nlohmann::json templateData;
    try {
        templateData = nlohmann::json::parse(file);
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return data;
    }

    if (templateData.contains(data.at("type"))) {
        auto defaultVal = templateData.at(data.at("type"));

        for (auto& [key, val] : data.items()) {
            defaultVal[key] = val;
        }
        return defaultVal;
    }

    return data;
}

void StateLoader::loadUI(const std::shared_ptr<IGameObject>& root) {
    std::filesystem::current_path(getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets" / "objects" / "GUIHolder.json";
    std::ifstream file(resPath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << resPath<< std::endl;
    }
    nlohmann::json data = nlohmann::json::parse(file);

    if (data.is_discarded()) {
        std::cerr << "Failed to parse JSON" << std::endl;
    }

    auto GUIHolder = GameObjectFactory::instance().createGameObject("VisualNode");
    loadChildObjects(GUIHolder, data);
    root->addChild(GUIHolder);
}

// main load function
std::shared_ptr<IGameObject> StateLoader::loadObject(const json& rootData) {
    auto root = GameObjectFactory::instance().createGameObject(rootData.at("type"));
    auto newData = loadObjectTemplate(root, rootData);
    
    addTransformComponent(root, newData);
    addMovementComponent(root, newData);
    addSpriteComponent(root, newData);
    addTextComponent(root, newData);
    addCollisionComponent(root, newData);
    loadChildObjects(root, newData);

    root->from_json(newData);
    return root;
}

void StateLoader::loadChildObjects(std::shared_ptr<IGameObject>& root, const json& data) {
    if (data.find("childs") != data.end() && data.at("childs").is_array()) {
        for (const auto& childJson : data.at("childs")) {
            root->addChild(loadObject(childJson));
        }
    }
}
    
std::vector<std::string> StateLoader::loadTextures(const std::string& stateId) {
    auto resPath = getTexturesConfigFile();

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

    auto textures = data[stateId];
    std::vector<std::string> textureIds;
    for (const auto& [name, path] : textures.items()) {
        std::filesystem::path fullPath = std::filesystem::canonical(std::filesystem::current_path() / "assets" / "sprites" / path.get<std::string>());
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
    auto resPath = getAudioConfigFile();

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

    auto sounds = data[stateId];
    for (const auto& [name, path] : sounds.items()) {
        auto fullPath = std::filesystem::canonical(std::filesystem::current_path() / "assets" / "sounds" / path.get<std::string>());
        AudioManager::instance().loadFile(fullPath.string(), name);
    }
}

void StateLoader::addTransformComponent(std::shared_ptr<IGameObject>& root, const json& data) {
    if (data.find("TransformComponent") != data.end()) {
        root->addComponent(std::make_shared<Transform2DComponent>(data["TransformComponent"]));
    }
}

void StateLoader::addMovementComponent(std::shared_ptr<IGameObject>& root, const json& data) {
    if (data.find("movementMode") != data.end()) {
        if (data["movementMode"] == "isometric") {
            auto movementComp = std::make_shared<MovementIsoComponent>();
            root->addComponent(movementComp);
            movementComp->setOwner(root);
        }
    }
}

void StateLoader::addSpriteComponent(std::shared_ptr<IGameObject>& root, const json& data) {
    if (data.find("image") != data.end()) {
        nlohmann::json spriteData = {
            { "width", data.value("width", 0) },
            { "height", data.value("height", 0) },
            { "image", data["image"] }
        };
        auto sprite = std::make_shared<Sprite2DComponent>(spriteData);
        root->addComponent(sprite);
        sprite->setOwner(root);
    }
    if (data.find("spriteComponent") != data.end()) {
        nlohmann::json spriteData = data["spriteComponent"];
        auto sprite = std::make_shared<Sprite2DComponent>(spriteData);
        root->addComponent(sprite);
        sprite->setOwner(root);
    }
}

void StateLoader::addTextComponent(std::shared_ptr<IGameObject>& root, const json& data) {
    if (data.find("text") != data.end()) {
        nlohmann::json textData = {
            { "width", data.value("width", 0) },
            { "height", data.value("height", 0) },
            { "text", data.value("text", "") },
            { "color", data["textColor"] }
        };
        root->addComponent(std::make_shared<TextComponent>(textData));
    }
}

void StateLoader::addCollisionComponent(std::shared_ptr<IGameObject>& root, const json& data) {
    if (data.find("CollisionComponent") != data.end()) {
        auto collider = std::make_shared<CollisionComponent>(
            data["CollisionComponent"].value("width", 10), 
            data["CollisionComponent"].value("height", 10)
        );
        root->addComponent(collider);
        collider->setOwner(root);
    }
}
}

