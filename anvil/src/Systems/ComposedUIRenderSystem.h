#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/MouseEvents.h"
#include "../Events/ButtonEvents.h"
#include "../components/ComposedUIComponent.h"
#include "../components/TransformComponent.h"
#include "../components/TextLabelComponent.h"
#include "../Logger/Logger.h"
#include "../Application.h"
#include "../states/game_state_machine.h"
#include "../states/playstate.h"
#include "../states/LoadingState.h"
#include "../Systems/CommandSystem.h"
#include "../Commands/SwitchScene.hpp"

namespace anvil {

class ComposedUIRenderSystem: public System {
    public:
        ComposedUIRenderSystem() {
            RequireComponent<RowUIComponent>();
            RequireComponent<TransformComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<MouseMotionEvent>(this, &ComposedUIRenderSystem::onMouseMotion);
            eventBus->SubscribeToEvent<MouseClickedEvent>([this, &eventBus](MouseClickedEvent& event) {
                for (auto entity: GetSystemEntities()) {
                    auto& row = entity.GetComponent<RowUIComponent>();
                    auto& transform = entity.GetComponent<TransformComponent>();
                    if (isMouseOverButton(event.x, event.y, row, transform)) {
                        row.state = State::CLICK;
                        if (entity.HasComponent<TextLabelComponent>()) {
                            auto& textComp = entity.GetComponent<TextLabelComponent>();
                            textComp.color = {30, 30, 46, 255};
                        }
                        eventBus->EmitEvent<ButtonClickedEvent>(entity);
                    }
                }
            });
        }

        unsigned int GetRowWidth(RowUIComponent& ui) {
            return ui.left.width + ui.mid.width + ui.right.width;
        }

        unsigned int GetRowHeight(RowUIComponent& ui) {
            return ui.left.height;
        }

        bool isMouseOverButton(int x, int y, RowUIComponent& ui, TransformComponent& transform) {
            return x >= transform.position.x && x <= transform.position.x + GetRowWidth(ui) &&
                   y >= transform.position.y && y <= transform.position.y + GetRowHeight(ui);
        }

        void onMouseMotion(MouseMotionEvent& event) {
            for (auto entity: GetSystemEntities()) {
                auto& row = entity.GetComponent<RowUIComponent>();
                if (isMouseOverButton(event.x, event.y, row, entity.GetComponent<TransformComponent>())) {
                    row.state = State::HOVER;
                    if (entity.HasComponent<TextLabelComponent>()) {
                        auto& textComp = entity.GetComponent<TextLabelComponent>();
                        textComp.color = {255, 255, 255, 255};
                    }
                }
                else {
                    row.state = State::DEFAULT;
                    if (entity.HasComponent<TextLabelComponent>()) {
                        auto& textComp = entity.GetComponent<TextLabelComponent>();
                        textComp.color = {160, 170, 210};
                    }
                }
            }
        }

        void onMouseClicked(MouseClickedEvent& event) {
            for (auto entity: GetSystemEntities()) {
                auto& row = entity.GetComponent<RowUIComponent>();
                auto& transform = entity.GetComponent<TransformComponent>();
                if (isMouseOverButton(event.x, event.y, row, transform)) {
                    row.state = State::CLICK;
                    if (entity.HasComponent<TextLabelComponent>()) {
                        auto& textComp = entity.GetComponent<TextLabelComponent>();
                        textComp.color = {30, 30, 46, 255};
                    }
                }
            }
        }


        void drawUICell(const std::shared_ptr<IRenderer>& renderer, const CellUIComponent& cell,
             const TransformComponent& transform, const std::unique_ptr<AssetStore>& assetStore,
             State state, float offset) {
                    SDL_FRect dstLeftRect = {
                        transform.position.x + offset, transform.position.y,
                        cell.width * transform.scale.x,
                        cell.height * transform.scale.y
                    };
                    SDL_FRect srcLeftRect = cell.srcRect;
                    std::shared_ptr<ITexture> texture;
                    switch (state) {
                        case State::DEFAULT:
                            if (cell.defaultAsset.has_value()) {
                                texture = assetStore->GetTexture(cell.defaultAsset.value());
                            } else {
                                return;
                            }
                            break;
                        case State::HOVER:
                            texture = assetStore->GetTexture(cell.hoverAsset.value_or(""));
                            break;
                        case State::CLICK:
                            texture = assetStore->GetTexture(cell.clickAsset.value_or(""));

                            break;
                    }

                    renderer->renderTextureRotated(
                        texture.get(), &srcLeftRect, &dstLeftRect, transform.rotation, nullptr, SDL_FLIP_NONE
                    );
        }

        void Update(std::shared_ptr<IRenderer> renderer, std::unique_ptr<AssetStore>& assetStore) {
            struct RenderableEntity {
                TransformComponent transformComponent;
                RowUIComponent rowUIComponent;
            };
            std::vector<RenderableEntity> renderableEntities; 
            for (auto entity: GetSystemEntities()) {
                RenderableEntity renderableEntity;
                renderableEntity.rowUIComponent = entity.GetComponent<RowUIComponent>();
                renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
                renderableEntities.emplace_back(renderableEntity);
            }

            for (auto entity: renderableEntities) {
                const auto transform = entity.transformComponent;
                const auto rowUI = entity.rowUIComponent;
                const auto scale = transform.scale;

                drawUICell(renderer, rowUI.left, transform,  assetStore, rowUI.state, 0.0f);
                drawUICell(renderer, rowUI.mid, transform,  assetStore, rowUI.state, rowUI.left.width * scale.x);
                drawUICell(renderer, rowUI.right, transform,  assetStore, rowUI.state, rowUI.left.width * scale.x + rowUI.mid.width * scale.x);
            }
        }
};

}