#include "json_serializer/stateloader.h"
#include <iostream>
#include "utils.h"
#include "texturemanager.h"
#include "renderer.h"
#include "game_objects/GameObjectsFactory.h"
#include "game_objects/gameobject.h"

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
    for (const auto& [child, params] : scene["childs"].items()) {

        if (params.is_array()) {
            for (auto& childParams : params) {
                auto childObj = GameObjectFactory::instance().createGameObject(child);
                childObj->from_json(childParams);
                sceneObject->addChildObject(std::move(childObj));
            }
        }
        else {
            auto childObj = GameObjectFactory::instance().createGameObject(child);
            childObj->from_json(params);
            sceneObject->addChildObject(std::move(childObj));
        }

    }
    return std::move(sceneObject);

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

}

