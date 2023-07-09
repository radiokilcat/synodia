#include "json_serializer/game_object_serializer.h"
#include "gameobject.h"

namespace anvil 
{
    void GameObjectSerializer::from_json(nlohmann::json& j, anvil::GameObject &object)
    {
        object.id_ = j["id_"];
        object.currentFrame_ = j["currentFrame_"];
        object.currentRow_ = j["currentRow_"];
        object.position_ = j["position_"];
        object.velocity_ = j["velocity_"];
        object.acceleration_ = j["acceleration_"];
        object.width_ = j["width_"];
        object.height_ = j["height_"];
    }

    void GameObjectSerializer::to_json(nlohmann::json& j, anvil::GameObject &object) {
        j["id_"] = object.id_;
        j["currentFrame_"] = object.currentFrame_;
        j["currentRow_"] = object.currentRow_;
        j["position_"] = object.position_;
        j["velocity_"] = object.velocity_;
        j["acceleration_"] = object.acceleration_;
        j["width_"] = object.width_;
        j["height_"] = object.height_;
    }
}