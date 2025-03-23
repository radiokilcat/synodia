#pragma once

#include "../ECS/ECS.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL3/SDL.h>
#include "../Application.h"

namespace anvil {

class CameraMovementSystem: public System {
    public:
        CameraMovementSystem() {
            RequireComponent<CameraFollowComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                auto transform = entity.GetComponent<TransformComponent>();
                auto windowWith = Application::Instance()->getScreenWidth();
                auto windowHeight = Application::Instance()->getScreenHeight();
                auto mapWidth = Application::Instance()->getMapWidth();
                auto mapHeight = Application::Instance()->getMapHeight();

                if (static_cast<int>(std::round(transform.position.x)) + (camera.w / 2) < mapWidth) {
                    camera.x = static_cast<int>(std::round(transform.position.x)) - (camera.w / 2);
                }

                if (static_cast<int>(std::round(transform.position.y)) + (camera.h / 2) < mapHeight) {
                    camera.y = static_cast<int>(std::round(transform.position.y)) - (camera.h / 2);
                }

                camera.x = camera.x < 0 ? 0 : camera.x;
                camera.y = camera.y < 0 ? 0 : camera.y;
                camera.x = (camera.x + camera.w > mapWidth) ? mapWidth - camera.w : camera.x;
                camera.y = (camera.y + camera.h > mapHeight) ? mapHeight - camera.h : camera.y;
            }
        }
};

}