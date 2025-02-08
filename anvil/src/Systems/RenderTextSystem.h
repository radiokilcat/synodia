#pragma once

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../components/TextLabelComponent.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

namespace anvil {

class RenderTextSystem: public System {
    public:
        RenderTextSystem() {
            RequireComponent<TextLabelComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto textlabel = entity.GetComponent<TextLabelComponent>();
                
                SDL_Surface* surface = TTF_RenderText_Blended(
                    assetStore->GetFont(textlabel.assetId),
                    textlabel.text.c_str(),
                    textlabel.color
                );
                if (!surface) {
                    Logger::Err("Failed to create surface for text label: " + textlabel.text);
                    throw std::runtime_error("Failed to create surface for text label: " + textlabel.text);
                    continue;
                }
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!texture) {
                    Logger::Err("Failed to create texture for text label: " + textlabel.text);
                    throw std::runtime_error("Failed to create texture for text label: " + textlabel.text);
                    continue;
                }
                SDL_DestroySurface(surface);

                int labelWidth = 0;
                int labelHeight = 0;

                SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

                SDL_FRect dstRect = {
                    textlabel.position.x - (textlabel.isFixed ? 0 : camera.x),
                    textlabel.position.y - (textlabel.isFixed ? 0 : camera.y),
                    static_cast<float>(labelWidth),
                    static_cast<float>(labelHeight)
                };

                SDL_RenderTexture(renderer, texture, NULL, &dstRect);

                SDL_DestroyTexture(texture);
            }
        }
};

}
