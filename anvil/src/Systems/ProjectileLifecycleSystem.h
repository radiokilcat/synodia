#pragma once

#include "../ECS/ECS.h"
#include "../components/ProjectileComponent.h"

namespace anvil {

class ProjectileLifecycleSystem: public System {
    public:
        ProjectileLifecycleSystem() {
            RequireComponent<ProjectileComponent>();
        }

        void Update() {
            for (auto entity: GetSystemEntities()) {
                auto projectile = entity.GetComponent<ProjectileComponent>();

                if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
                    entity.Kill();
                }
            }
        }
};

}