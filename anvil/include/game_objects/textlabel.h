#pragma once

#include <SDL3/SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "renderer.h"
#include "gameobject.h"
#include "fontloader.h"


namespace anvil {
    class TextComponent;
    class Transform2DComponent;
    class Sprite2DComponent;

    class TextLabel : public GameObject
{
public:
    TextLabel();
    TextLabel(const nlohmann::json& data);
    ~TextLabel() {};

    void draw(std::shared_ptr<Renderer> renderer) override;
    void drawWrapped(std::shared_ptr<Renderer> renderer, Uint32 wrapLength);
    void setText(const std::string& text);
    void update(Uint64 deltaTime) override {};
    void clean() override {};
    void init() override;
    
    void from_json(const nlohmann::json& j);
    void to_json(nlohmann::json& j);

    static bool registerWithFactory();
    
private:
    std::shared_ptr<anvil::Sprite2DComponent> sprite_;
    std::shared_ptr<anvil::Transform2DComponent> transform_;
    std::shared_ptr<anvil::TextComponent> text_;
};

}
