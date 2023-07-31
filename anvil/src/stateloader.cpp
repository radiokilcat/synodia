#include "json_serializer/stateloader.h"
#include <iostream>
#include "utils.h"
#include "texturemanager.h"
#include "renderer.h"

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
    for (auto& [name, path] : textures.items()) {
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

