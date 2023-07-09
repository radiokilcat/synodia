#pragma once
#include <nlohmann/json.hpp>
#include "game_object_serializer.h"
#include "menustate.h"

namespace anvil {
    class MenuStateSerializer
    {
    public:
        MenuStateSerializer(GameObjectSerializer gameObjectSerializer);
        void from_json(nlohmann::json& j, MenuState& object);

        void to_json(nlohmann::json& j, MenuState& object);
    private:
        GameObjectSerializer m_gameObjectSerializer;
    };
}