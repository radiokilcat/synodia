#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include <imgui.h>
#include "backends/imgui_impl_sdlrenderer3.h"
#include "backends/imgui_impl_sdl3.h"

namespace anvil {
    // Deprecated; use ImGuiSystem instead

class RenderImGUISystem: public System {
    public:
        RenderImGUISystem() = default;

        void Update(const std::unique_ptr<Registry>& registry, const SDL_Rect& camera) {
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            // Use a collapsible header for enemy spawning
            if (ImGui::CollapsingHeader("Spawn enemies", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Static variables to hold input values
                static int posX = 0;
                static int posY = 0;
                static int scaleX = 1;
                static int scaleY = 1;
                static int velX = 0;
                static int velY = 0;
                static int health = 100;
                static float rotation = 0.0f;
                static float projAngle = 0.0f;
                static float projSpeed = 100.0f;
                static int projRepeat = 10;
                static int projDuration = 10;
                const char* sprites[] = {"tank-image", "truck-image"};
                static int selectedSpriteIndex = 0;

                // Section to input enemy sprite texture id 
                if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
                }
                ImGui::Spacing();

                // Section to input enemy transform values
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("position x", &posX);
                    ImGui::InputInt("position y", &posY);
                    ImGui::SliderInt("scale x", &scaleX, 1, 10);
                    ImGui::SliderInt("scale y", &scaleY, 1, 10);
                    ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
                }
                ImGui::Spacing();

                // Section to input enemy rigid body values
                if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::InputInt("velocity x", &velX);
                    ImGui::InputInt("velocity y", &velY);
                }
                ImGui::Spacing();

                // Section to input enemy projectile emitter values
                if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
                    ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
                    ImGui::InputInt("repeat (sec)", &projRepeat);
                    ImGui::InputInt("duration (sec)", &projDuration);
                }
                ImGui::Spacing();

                // Section to input enemy health values
                if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::SliderInt("%", &health, 0, 100);
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                if (ImGui::Button("Spawn new enemy")) {
                    Entity enemy = registry->CreateEntity();
                    enemy.Group("enemies");
                    enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                    enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                    enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 2);
                    enemy.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
                    double projVelX = cos(projAngle) * projSpeed; // convert from angle-speed to x-value
                    double projVelY = sin(projAngle) * projSpeed; // convert from angle-speed to y-value
                    enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false);
                    enemy.AddComponent<HealthComponent>(health);

                    posX = posY = 0;
                    rotation = projAngle = 0.0f;
                    scaleX = scaleY = 1;
                    projRepeat = projDuration = 10;
                    projSpeed = 100.0f;
                    health = 100;
                }
            }

            if (ImGui::Begin("Entities and Components")) {
                for (auto& entity : registry->GetEntities()) {
                    ImGui::Text("Entity ID: %d", entity.GetId());
                    
                    if (entity.HasComponent<TransformComponent>()) {
                        auto& transform = entity.GetComponent<TransformComponent>();
                        ImGui::BulletText("Transform: pos(%.1f, %.1f), scale(%.1f, %.1f), rotation(%.1f)",
                            transform.position.x, transform.position.y,
                            transform.scale.x, transform.scale.y,
                            transform.rotation);
                    }
                    if (entity.HasComponent<RigidBodyComponent>()) {
                        auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
                        ImGui::BulletText("RigidBody: velocity(%.1f, %.1f)",
                            rigidBody.velocity.x, rigidBody.velocity.y);
                    }
                    if (entity.HasComponent<SpriteComponent>()) {
                        auto& sprite = entity.GetComponent<SpriteComponent>();
                        ImGui::BulletText("Sprite: texture id: %s", sprite.assetId.c_str());
                    }
                    if (entity.HasComponent<BoxColliderComponent>()) {
                        auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
                        ImGui::BulletText("BoxCollider: width: %d, height: %d", boxCollider.width, boxCollider.height);
                    }
                    // if (entity.HasComponent<ProjectileEmitterComponent>()) {
                    //     auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                    //     ImGui::BulletText("ProjectileEmitter: velocity(%.1f, %.1f)",
                    //         projEmitter.velocity.x, projEmitter.velocity.y);
                    // }
                    if (entity.HasComponent<HealthComponent>()) {
                        auto& healthComp = entity.GetComponent<HealthComponent>();
                        ImGui::BulletText("Health: %d", healthComp.healthPercentage);
                    }
                    if (entity.HasComponent<CameraFollowComponent>()) {
                        ImGui::BulletText("Camera follow: true");
                    }
                    if (entity.HasComponent<KeyboardControlledComponent>()) {
                        ImGui::BulletText("KeyboardController: upVelocity: %d, %d, rightVelocity: %d, %d, downVelocity: %d, %d, leftVelocity: %d, %d",
                            entity.GetComponent<KeyboardControlledComponent>().upVelocity.x,
                            entity.GetComponent<KeyboardControlledComponent>().upVelocity.y,
                            entity.GetComponent<KeyboardControlledComponent>().rightVelocity.x,
                            entity.GetComponent<KeyboardControlledComponent>().rightVelocity.y,
                            entity.GetComponent<KeyboardControlledComponent>().downVelocity.x,
                            entity.GetComponent<KeyboardControlledComponent>().downVelocity.y,
                            entity.GetComponent<KeyboardControlledComponent>().leftVelocity.x,
                            entity.GetComponent<KeyboardControlledComponent>().leftVelocity.y);
                    }
                    ImGui::Separator();
                }
            }
            ImGui::End();

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.9f);
            if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
                ImGui::Text("Map coordinates (x=%.1f, y=%.1f)",
                    ImGui::GetIO().MousePos.x + camera.x,
                    ImGui::GetIO().MousePos.y + camera.y);
            }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        }
};

}
