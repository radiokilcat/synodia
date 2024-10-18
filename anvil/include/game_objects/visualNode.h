#pragma once

#include "anvil.h"
#include "components/Sprite2DComponent.h"

namespace anvil
{
    class VisualNode : public anvil::GameObject
    {
    public:
        VisualNode() = default;
        VisualNode(int layer, bool outline);

        void draw(std::shared_ptr<anvil::Renderer> renderer) override;
        void update(Uint64 deltaTime) override;
        void init() override;
        void clean() override;

        void from_json(const nlohmann::json& j);
        void to_json(nlohmann::json& j);

        static bool registerWithFactory();

    protected:
        std::shared_ptr<anvil::Sprite2DComponent> sprite_;
        std::shared_ptr<anvil::Transform2DComponent> transform_;
    };
}