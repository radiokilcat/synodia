#pragma once

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../components/TextLabelComponent.h"
#include "../components/TransformComponent.h"
#include "../Render/IRenderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

namespace anvil {

class RenderTextSystem: public System {
    public:
        RenderTextSystem() {
            RequireComponent<TextLabelComponent>();
        }

        void Update(std::shared_ptr<IRenderer> renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto textlabel = entity.GetComponent<TextLabelComponent>();
                auto label = renderer->createTextTexture(textlabel.text, assetStore->GetFont(textlabel.assetId), textlabel.color);

                int labelWidth = label->getWidth();
                int labelHeight = label->getHeight();

                float x_ = textlabel.position.x;
                float y_ =  textlabel.position.y;

                if (textlabel.isNested && entity.HasComponent<TransformComponent>()) {
                    const auto transform = entity.GetComponent<TransformComponent>();
                    labelWidth *= transform.scale.x;
                    labelHeight *= transform.scale.y;
                    x_ += transform.position.x;
                    y_ += transform.position.y;
                }

                SDL_FRect dstRect = {
                    x_ - (textlabel.isFixed ? 0 : camera.x),
                    y_ - (textlabel.isFixed ? 0 : camera.y),
                    static_cast<float>(labelWidth),
                    static_cast<float>(labelHeight)
                };

                renderer->renderTextureRotated(label.get(), nullptr, &dstRect, 0.0, nullptr, SDL_FLIP_NONE);
            }
        }
};

}
