#pragma once

#include <string>
#include <memory>
#include "nlohmann/json.hpp"
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"

using json = nlohmann::json;

namespace anvil {

class StateLoader {
public:

    StateLoader(const std::unique_ptr<Registry>& reg);
    bool loadFromFile(const std::string& filename);
    json readJsonContent(const std::string &filename);
    void loadResources(std::shared_ptr<IRenderer> renderer, const std::string &filename, const std::unique_ptr<AssetStore> &assetStore);

private:
    void loadTransformComponent(Entity& entity, const json& components);
    void loadSpriteComponent(Entity& entity, const json& components);
    void loadAnimationComponent(Entity& entity, const json& components);
    void loadBoxColliderComponent(Entity& entity, const json& components);
    void loadHealthComponent(Entity& entity, const json& components);
    void loadRigidBodyComponent(Entity& entity, const json& components);
    void loadTextLabelComponent(Entity& entity, const json& components);
    void loadProjectileComponent(Entity& entity, const json& components);
    void loadProjectileEmitterComponent(Entity& entity, const json& components);
    void loadKeyboardControlledComponent(Entity& entity, const json& components);
    void loadCameraFollowComponent(Entity& entity, const json& components);
    void loadTileMap(const json& components);
    void loadRowUIComponent(Entity &entity, const json &components);

    const std::unique_ptr<Registry>& registry;
    void loadEntities(const json& stateJson);
};

}
