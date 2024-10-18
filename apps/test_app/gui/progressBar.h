#pragma once

#include "anvil.h"
#include "components/TextComponent.h"

class GUIProgressBar : public anvil::GameObject
{
public:
    GUIProgressBar() = default;

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void init() override;

    void from_json(const nlohmann::json& j);
    void to_json(nlohmann::json& j);

    static bool registerWithFactory();
};
