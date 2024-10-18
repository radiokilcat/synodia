#include "game_objects/visualNode.h"
#include "nlohmann/json.hpp"

namespace anvil
{
    class Sprite2DComponent;

    VisualNode::VisualNode(int layer, bool outline) {
        layer_ = layer;
        outline_ = outline;
    }

    void VisualNode::draw(std::shared_ptr<anvil::Renderer> renderer) {
        if (transform_ && sprite_) {
            auto x = transform_->getX();
            auto y = transform_->getY();
            sprite_->drawScaled(x, y, renderer);
        }
        for (const auto& child: m_children ) {
            child->draw(renderer);
        }
    }

    void VisualNode::update(Uint64 deltaTime) {
    }

    void VisualNode::init() {
        if (!sprite_) {
            sprite_ = getComponent<anvil::Sprite2DComponent>();
        }
        if (!transform_) {
            transform_ = getComponent<anvil::Transform2DComponent>();
        }
        if (sprite_) {
            sprite_->setState("idle");
        }
    }

    void VisualNode::clean()
    {
    }

    void VisualNode::from_json(const nlohmann::json& j) {
        id_ = j.value("id", std::string("test1"));
        layer_ = j.value("layer", 0);
        // color_ = getColor(j);
        // text_ = j.value("text", "");
    }

    void VisualNode::to_json(nlohmann::json& j) {
        j["id"] = id_;
        j["layer"] = layer_;
        // j["text"] = text_;
    }

    bool VisualNode::registerWithFactory()
    {
        anvil::GameObjectFactory::instance().registerType("VisualNode", []() -> std::unique_ptr<anvil::IGameObject> {
            return std::make_unique<VisualNode>();
        });
        return true;
    }
}