#include "json_serializer/menu_state_serializer.h"
#include "menustate.h"

namespace anvil 
{
    MenuStateSerializer::MenuStateSerializer(GameObjectSerializer gameObjectSerializer) : m_gameObjectSerializer(gameObjectSerializer)
    {}

    void MenuStateSerializer::from_json(nlohmann::json& j, anvil::MenuState& object)
    {
        for(auto childJ : j["m_gameObjects"]) {
            GameObject *childObj = new GameObject;
            m_gameObjectSerializer.from_json(childJ, *childObj);
            object.m_gameObjects.push_back(childObj);
        }
    }

    void MenuStateSerializer::to_json(nlohmann::json& j, anvil::MenuState& object) {
        for(auto gameObject : object.m_gameObjects)
        {
            json childJ;
            m_gameObjectSerializer.to_json(childJ, *gameObject);
            j["m_gameObjects"].push_back(childJ);
        }
    }
}