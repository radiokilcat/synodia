#pragma once
#include <nlohmann/json.hpp>
#include "serializable_base.h"
#include "gameobject.h"

namespace anvil {
    class GameObjectSerializer
    {
    public:
        void from_json(nlohmann::json& j, GameObject& object);

        void to_json(nlohmann::json& j, GameObject& object);
    };
}