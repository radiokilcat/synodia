#include "stateLoader.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../application.h"
#include "../Logger/Logger.h"
#include "../components/AnimationComponent.h"
#include "../components/BoxColliderComponent.h"
#include "../components/HealthComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/TextLabelComponent.h"
#include "../components/TransformComponent.h"
#include "../components/ProjectileComponent.h"
#include "../components/ProjectileEmitterComponent.h"
#include "../components/KeyboardControlledComponent.h"
#include "../components/CameraFollowComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderImGUISystem.h"
#include "../Systems/ButtonSystem.h"

using json = nlohmann::json;

namespace anvil {

json mergeJson(const json& base, const json& override) {
    json merged = base;

    for (auto& [key, value] : override.items()) {
        if (merged.contains(key) && merged[key].is_object() && value.is_object()) {
            merged[key] = mergeJson(merged[key], value);
        } else {
            merged[key] = value;
        }
    }

    return merged;
}

glm::vec2 parsePosition(const json& j, int spriteW = 0, int spriteH = 0) {
    if (j.contains("position")) {
        if (j["position"].is_string()) {
            std::string pos = j["position"];
            if (pos == "center") {
                int screenW = Application::Instance()->getScreenWidth();
                int screenH = Application::Instance()->getScreenHeight();
                return glm::vec2(screenW / 2, screenH / 2);
            }
        }

        glm::vec2 position = glm::vec2(0);

        if (j["position"]["x"].is_string() ) {
            std::string x = j["position"]["x"];
            if (x == "center") {
                int screenW = Application::Instance()->getScreenWidth();
                position.x = (screenW / 2) - (spriteW / 2);
            }
        }
        if (j["position"]["y"].is_string()) {
            std::string y = j["position"]["y"];
            if (y == "center") {
                int screenH = Application::Instance()->getScreenHeight();
                position.y = (screenH / 2) - (spriteH / 2);
            }
        }
        if (j["position"]["x"].is_number()) {
            position.x = j["position"]["x"];
        }
        if (j["position"]["y"].is_number()) {
            position.y = j["position"]["y"];
        }
        return position;
    }
    return glm::vec2(0);
}

StateLoader::StateLoader(const std::unique_ptr<Registry>& reg) : registry(reg) {}

bool StateLoader::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::Err("Failed to open state file: " + filename);
        return false;
    }

    json stateJson;
    try {
        file >> stateJson;
    } catch (json::parse_error& e) {
        Logger::Err("JSON parse error: " + std::string(e.what()));
        return false;
    }


    loadEntities(stateJson);
    return true;
}

void StateLoader::loadResources(SDL_Renderer* renderer, const std::string& filename, const std::unique_ptr<AssetStore>& assetStore) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::Err("Failed to open asset file: " + filename);
        return;
    }

    json assetJson;
    try {
        file >> assetJson;
    } catch (json::parse_error& e) {
        Logger::Err("JSON parse error: " + std::string(e.what()));
        return;
    }

    if (assetJson.contains("textures")) {
        for (auto& [key, value] : assetJson["textures"].items()) {
            assetStore->AddTexture(renderer, key, value);
        }
    }

    if (assetJson.contains("fonts")) {
        for (auto& [key, value] : assetJson["fonts"].items()) {
            assetStore->AddFont(key, value, 24); // Assuming a default font size of 24
        }
    }
}

void StateLoader::loadEntities(const json& stateJson) {
    for (const auto& entityDef : stateJson) {
        if (entityDef.contains("type") && entityDef["type"] == "tilemap") {
            loadTileMap(entityDef);
            continue;
        }
        Entity newEntity = registry->CreateEntity();
        
        if (entityDef.contains("tags")) {
            for (const auto& tag : entityDef["tags"]) {
                registry->TagEntity(newEntity, tag);
            }
        }

        if (entityDef.contains("groups")) {
            for (const auto& group : entityDef["groups"]) {
                registry->GroupEntity(newEntity, group);
            }
        }

        auto& components = entityDef["components"];

        loadTransformComponent(newEntity, components);
        loadSpriteComponent(newEntity, components);
        loadAnimationComponent(newEntity, components);
        loadBoxColliderComponent(newEntity, components);
        loadHealthComponent(newEntity, components);
        loadRigidBodyComponent(newEntity, components);
        loadTextLabelComponent(newEntity, components);
        loadProjectileComponent(newEntity, components);
        loadProjectileEmitterComponent(newEntity, components);
        loadKeyboardControlledComponent(newEntity, components);
        loadCameraFollowComponent(newEntity, components);
    }
}

void StateLoader::loadTransformComponent(Entity& entity, const json& components) {
    if (components.contains("transform")) {
        auto& transform = components["transform"];
        if (components.contains("sprite")
        && components["sprite"].contains("width")
        && components["sprite"].contains("height")) {
            auto spriteW = components["sprite"]["width"].get<int>();
            auto spriteH = components["sprite"]["height"].get<int>();
            entity.AddComponent<TransformComponent>(
                parsePosition(transform, spriteW, spriteH),
                glm::vec2(
                    transform.contains("scale") ? transform["scale"]["x"].get<float>() : 1.0f,
                    transform.contains("scale") ? transform["scale"]["y"].get<float>() : 1.0f
                ),
                transform.contains("rotation") ? transform["rotation"].get<double>() : 0.0
            );
        }
        else {
            entity.AddComponent<TransformComponent>(
                parsePosition(transform),
                glm::vec2(
                    transform.contains("scale") ? transform["scale"]["x"].get<float>() : 1.0f,
                    transform.contains("scale") ? transform["scale"]["y"].get<float>() : 1.0f
                ),
                transform.contains("rotation") ? transform["rotation"].get<double>() : 0.0
            );
        }
    }
}

void StateLoader::loadSpriteComponent(Entity& entity, const json& components) {
    if (components.contains("sprite")) {
        auto& sprite = components["sprite"];
        entity.AddComponent<SpriteComponent>(
            sprite["assetId"],
            sprite["width"],
            sprite["height"],
            sprite.contains("zIndex") ? sprite["zIndex"].get<int>() : 0,
            sprite.contains("isFixed") ? sprite["isFixed"].get<bool>() : false,
            sprite.contains("srcRectX") ? sprite["srcRectX"].get<int>() : 0,
            sprite.contains("srcRectY") ? sprite["srcRectY"].get<int>() : 0
        );
    }
}

void StateLoader::loadAnimationComponent(Entity& entity, const json& components) {
    if (components.contains("animation")) {
        auto& anim = components["animation"];
        entity.AddComponent<AnimationComponent>(
            anim.contains("frameRate") ? anim["frameRate"].get<int>() : 1,
            anim.contains("loop") ? anim["loop"].get<bool>() : true
        );
    }
}

void StateLoader::loadBoxColliderComponent(Entity& entity, const json& components) {
    if (components.contains("boxCollider")) {
        auto& collider = components["boxCollider"];
        entity.AddComponent<BoxColliderComponent>(
            collider["width"],
            collider["height"],
            glm::vec2(
                collider.contains("offset") ? collider["offset"]["x"].get<float>() : 0.0f,
                collider.contains("offset") ? collider["offset"]["y"].get<float>() : 0.0f
            )
        );
    }
}

void StateLoader::loadHealthComponent(Entity& entity, const json& components) {
    if (components.contains("health")) {
        auto& health = components["health"];
        entity.AddComponent<HealthComponent>(
            health["healthPercentage"]
        );
    }
}

void StateLoader::loadRigidBodyComponent(Entity& entity, const json& components) {
    if (components.contains("rigidBody")) {
        auto& rb = components["rigidBody"];
        entity.AddComponent<RigidBodyComponent>(
            glm::vec2(
                rb["velocity"]["x"],
                rb["velocity"]["y"]
            )
        );
    }
}

void StateLoader::loadTextLabelComponent(Entity& entity, const json& components) {
    if (components.contains("textLabel")) {
        auto& label = components["textLabel"];
        SDL_Color color = {
            static_cast<Uint8>(label.contains("color") ? label["color"][0].get<int>() : 0),
            static_cast<Uint8>(label.contains("color") ? label["color"][1].get<int>() : 0),
            static_cast<Uint8>(label.contains("color") ? label["color"][2].get<int>() : 0),
            static_cast<Uint8>(label.contains("color") ? label["color"][3].get<int>() : 255)
        };
        entity.AddComponent<TextLabelComponent>(
            parsePosition(label),
            label["text"],
            label["assetId"],
            color,
            label.contains("isNested") ? label["isNested"].get<bool>() : false,
            label.contains("isFixed") ? label["isFixed"].get<bool>() : true
        );
    }
}

void StateLoader::loadProjectileComponent(Entity& entity, const json& components) {
    if (components.contains("projectile")) {
        auto& proj = components["projectile"];
        entity.AddComponent<ProjectileComponent>(
            proj.contains("isFriendly") ? proj["isFriendly"].get<bool>() : false,
            proj.contains("hitPercentDamage") ? proj["hitPercentDamage"].get<int>() : 0,
            proj.contains("duration") ? proj["duration"].get<int>() : 0
        );
    }
}

void StateLoader::loadProjectileEmitterComponent(Entity& entity, const json& components) {
    if (components.contains("projectileEmitter")) {
        auto& emitter = components["projectileEmitter"];
        entity.AddComponent<ProjectileEmitterComponent>(
            glm::vec2(
                emitter["projectileVelocity"]["x"],
                emitter["projectileVelocity"]["y"]
            ),
            emitter["repeatFrequency"],
            emitter.contains("projectileDuration") ? emitter["projectileDuration"].get<int>() : 10000,
            emitter.contains("hitPercentDamage") ? emitter["hitPercentDamage"].get<int>() : 10,
            emitter.contains("isFriendly") ? emitter["isFriendly"].get<bool>() : false
        );
    }
}

void StateLoader::loadKeyboardControlledComponent(Entity& entity, const json& components) {
    if (components.contains("keyboardControlled")) {
        auto& keyboard = components["keyboardControlled"];
        entity.AddComponent<KeyboardControlledComponent>(
            glm::vec2(keyboard["upVelocity"]["x"].get<float>(), keyboard["upVelocity"]["y"].get<float>()),
            glm::vec2(keyboard["rightVelocity"]["x"].get<float>(), keyboard["rightVelocity"]["y"].get<float>()),
            glm::vec2(keyboard["downVelocity"]["x"].get<float>(), keyboard["downVelocity"]["y"].get<float>()),
            glm::vec2(keyboard["leftVelocity"]["x"].get<float>(), keyboard["leftVelocity"]["y"].get<float>())
        );
    }
}

void StateLoader::loadCameraFollowComponent(Entity& entity, const json& components) {
    if (components.contains("cameraFollow")) {
        entity.AddComponent<CameraFollowComponent>();
    }
}

void StateLoader::loadTileMap(const json& element) {
    if (element.contains("tilemap")) {
        auto& tilemapObj = element["tilemap"];
        int tileWidth = tilemapObj["tileWidth"];
        int tileHeight = tilemapObj["tileHeight"];
        double tileScale = tilemapObj["tileScale"];
        int mapNumCols = tilemapObj["mapNumCols"];
        int mapNumRows = tilemapObj["mapNumRows"];
        int m_startX = tilemapObj["startPosition"]["x"].get<int>();
        int m_startY = tilemapObj["startPosition"]["y"].get<int>();


        // for (int y = 0; y < mapNumRows; y++) {
        //     for (int x = 0; x < mapNumCols; x++) {
        //         auto tileX = m_startX + (x - y) * tileWidth / 2;
        //         auto tileY = m_startY + (x + y) * tileHeight / 2;
        //         auto srcRectX = 0;
        //         auto srcRectY = 0;
        //         auto posX = x * (tileScale * tileSize);
        //         auto posY = y * (tileScale * tileSize);

        // SDL_FPoint p1 = { tileX + m_tileWidth / 2, tileY };
        // SDL_FPoint p2 = { tileX + m_tileWidth / 2 + m_tileWidth / 2, tileY  + m_tileHeight / 2 };
        // SDL_FPoint p3 = { tileX + m_tileWidth / 2, tileY  + m_tileHeight };
        // SDL_FPoint p4 = { tileX - m_tileWidth / 2 + m_tileWidth / 2, tileY + m_tileHeight / 2 };


        //         Entity tile = registry->CreateEntity();
        //         tile.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(tileScale, tileScale), 0.0);
        //         tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        //     }
        // }
    }

}
}
