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

                const Uint64 now = SDL_GetTicks();
                if (now - projectile.startTime > static_cast<Uint64>(projectile.duration)) {
                    entity.Kill();
                }
            }
        }
};

}