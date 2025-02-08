#pragma once

#include "fontloader.h"
#include "IComponent.h"
#include "nlohmann/json.hpp"
#include "texturemanager.h"
#include "game_objects/gameobject.h"

namespace anvil {
    
struct Color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

class TextComponent: public IOLDComponent {
public:
    TextComponent(std::string text, Color color, TTF_Font* font = FontLoader::instance()->getDefaultFont());
    TextComponent(const nlohmann::json& data);
	
	void draw(float x, float y, std::shared_ptr<Renderer> renderer);
	// void drawScaled(float x, float y, std::shared_ptr<Renderer> renderer);
    void drawWrapped(float x, float y, std::shared_ptr<Renderer> renderer, Uint32 wrapLength);
	
    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setText(const std::string& text);
    void update(Uint64 deltaTime) override {};
	void init() override {};
    
    void from_json(const nlohmann::json& j);
    void to_json(const nlohmann::json& j);

    void setTextScale(int size);
private:
	std::string text_;
    TTF_Font* font_ = nullptr;
    int textScale_ = 1;
    SDL_Color color_;
    float width_;
    float height_;
};

}
