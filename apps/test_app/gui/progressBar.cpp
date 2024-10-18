#include "progressBar.h"
#include "nlohmann/json.hpp"

namespace anvil {
    class Sprite2DComponent;
}

void GUIProgressBar::draw(std::shared_ptr<anvil::Renderer> renderer) {
    for (const auto& child: m_children ) {
        child->draw(renderer);
    }
}


void GUIProgressBar::init() {
}

void GUIProgressBar::from_json(const nlohmann::json& j) {
    id_ = j.value("id", std::string("test1"));
    layer_ = j.value("layer", 0);
}

void GUIProgressBar::to_json(nlohmann::json& j) {
    j["id"] = id_;
    j["layer"] = layer_;
}

bool GUIProgressBar::registerWithFactory()
{
    anvil::GameObjectFactory::instance().registerType("GUIProgressBar", []() -> std::unique_ptr<anvil::IGameObject> {
        return std::make_unique<GUIProgressBar>();
    });
    return true;
}
