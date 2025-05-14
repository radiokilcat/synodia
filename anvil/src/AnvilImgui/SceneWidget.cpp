#include "SceneWidget.h"

#include "imgui.h"
#include "../application.h"
#include "../states/game_state_machine.h"
#include "../ECS/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/HealthComponent.h"
#include "../components/ProjectileEmitterComponent.h"
#include "../components/KeyboardControlledComponent.h"

namespace anvil {

void GameSceneWidget::draw() {
    propertyWindow(true);
}

void GameSceneWidget::propertyWindow(bool p_open) {
    ImGui::Begin("test", NULL, ImGuiWindowFlags_None);
    if (ImGui::BeginTable("##split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable |
                                        ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Entities");
        ImGui::TableSetupColumn("Properties");
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();

        const auto& entities = Application::Instance()->getStateMachine()->getActiveState()->getRegistry()->GetEntities();
        for (const auto& e : entities) {
            listEntity(e);
        }

        ImGui::TableNextColumn();
        if (m_openEntityId != -1) {
            for (const auto& e : entities) {
                if (e.GetId() == m_openEntityId) {
                    componentsProperties(e);
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void GameSceneWidget::listEntity(const Entity& entity) {
    {
        ImGui::PushID(entity.GetId());
        std::string label = "entity_" + std::to_string(entity.GetId());

        bool isOpen = false;

        if (entity.GetId() == m_openEntityId) {
            isOpen = true;
        }
        ImGui::SetNextItemOpen(isOpen, ImGuiCond_Always);
    
        bool node_open = ImGui::TreeNodeEx(
            label.c_str(),
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth
        );

        // bool isOpen = (entity.GetId() == m_openEntityId);
        if (ImGui::IsItemClicked()) {
            m_openEntityId = entity.GetId();
        }
    
        if (node_open) {
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
}

void GameSceneWidget::componentsProperties(const Entity& entity) {
    ImGui::SetCursorPosY(ImGui::GetTextLineHeight() * 2);

    if (entity.HasComponent<TransformComponent>()) {
        drawTransformComponent(entity.GetComponent<TransformComponent>());
    }

    if (entity.HasComponent<SpriteComponent>()) {
        drawSpriteComponent(entity.GetComponent<SpriteComponent>());
    }

    if (entity.HasComponent<AnimationComponent>()) {
        drawAnimationComponent(entity.GetComponent<AnimationComponent>());
    }

    if (entity.HasComponent<RigidBodyComponent>()) {
        drawRigidBodyComponent(entity.GetComponent<RigidBodyComponent>());
    }

    if (entity.HasComponent<HealthComponent>()) {
        drawHealthComponent(entity.GetComponent<HealthComponent>());
    }

    if (entity.HasComponent<ProjectileEmitterComponent>()) {
        drawProjectileEmitterComponent(entity.GetComponent<ProjectileEmitterComponent>());
    }

    if (entity.HasComponent<KeyboardControlledComponent>()) {
        drawKeyboardControlledComponent(entity.GetComponent<KeyboardControlledComponent>());
    }

    if (entity.HasComponent<TextLabelComponent>()) {
        drawTextComponent(entity.GetComponent<TextLabelComponent>());
    }
}

void GameSceneWidget::drawTransformComponent(TransformComponent& transform) {
    ImGui::SeparatorText("TransformComponent");
    float position[2] = {transform.position.x, transform.position.y};
    float scale[2] = {transform.scale.x, transform.scale.y};
    double rotation = transform.rotation;

    if (ImGui::InputFloat2("Position", position)) {
        // transform.position.x = x;
    }
    if (ImGui::InputFloat("Scale", scale)) {
        // transform.scale.x = scaleX;
    }
    if (ImGui::InputDouble("Rotation", &rotation)) {
        transform.rotation = rotation;
    }
}

void GameSceneWidget::drawSpriteComponent(SpriteComponent& sprite) {
    ImGui::SeparatorText("SpriteComponent");
    int size[2] = {sprite.width, sprite.height};
    std::string assetId = sprite.assetId;

    if (ImGui::InputInt2("Width", size)) {
        // sprite.width = width;
    }
    char buffer[256];
    std::strncpy(buffer, assetId.c_str(), sizeof(buffer));
    if (ImGui::InputText("Asset ID", buffer, sizeof(buffer))) {
        sprite.assetId = std::string(buffer);
    }
}

void GameSceneWidget::drawAnimationComponent(AnimationComponent &animation) {
    ImGui::SeparatorText("AnimationComponent");
    int frameSpeedRate = animation.frameSpeedRate;
    bool isLoop = animation.isLoop;
    int currentFrame = animation.currentFrame;
    if (ImGui::InputInt("Frame Speed Rate", &frameSpeedRate)) {
        // animation.frameSpeedRate = frameSpeedRate;
    }
    if (ImGui::Checkbox("Is Loop", &isLoop)) {
        // animation.isLoop = isLoop;
    }
    if (ImGui::InputInt("Current Frame", &currentFrame)) {
        // animation.framesInRow = framesInRow;
    }
    int framePos[2] = {animation.frameX, animation.frameY};
    if (ImGui::InputInt2("srcPos", framePos)) {
        // animation.framesInRow = framesInRow;
    }
}

void GameSceneWidget::drawRigidBodyComponent(RigidBodyComponent& rigidBody) {
    ImGui::SeparatorText("RigidBodyComponent");
    float velocity[2] = {rigidBody.velocity.x, rigidBody.velocity.y};
    if (ImGui::InputFloat2("Velocity X", velocity)) {
        // rigidBody.velocity.x = velocityX;
    }
}

void GameSceneWidget::drawHealthComponent(HealthComponent& health) {
    ImGui::SeparatorText("HealthComponent");
    int healthPercentage = health.healthPercentage;

    if (ImGui::InputInt("Health Percentage", &healthPercentage)) {
        health.healthPercentage = healthPercentage;
    }
}

void GameSceneWidget::drawProjectileEmitterComponent(ProjectileEmitterComponent& emitter) {
    ImGui::SeparatorText("ProjectileEmitterComponent");
    float velocity[2] = {emitter.projectileVelocity.x, emitter.projectileVelocity.y};
    int repeatFrequency = emitter.repeatFrequency;
    int duration = emitter.projectileDuration;
    int damage = emitter.hitPercentDamage;

    if (ImGui::InputFloat2("Velocity X", velocity)) {
        // emitter.projectileVelocity.x = velocityX;
    }
    if (ImGui::InputInt("Repeat Frequency", &repeatFrequency)) {
        emitter.repeatFrequency = repeatFrequency;
    }
    if (ImGui::InputInt("Duration", &duration)) {
        emitter.projectileDuration = duration;
    }
    if (ImGui::InputInt("Damage", &damage)) {
        emitter.hitPercentDamage = damage;
    }
}

void GameSceneWidget::drawKeyboardControlledComponent(KeyboardControlledComponent& keyboard) {
    ImGui::SeparatorText("KeyboardControlledComponent");

    glm::vec2 velocities[] = {keyboard.upVelocity, keyboard.rightVelocity, keyboard.downVelocity, keyboard.leftVelocity};
    const char* directions[] = {"Up", "Right", "Down", "Left"};

    for (int i = 0; i < 4; i++) {
        float vel[2] = {velocities[i].x, velocities[i].y};
        if (ImGui::InputFloat2(directions[i], vel, "%.2f")) {
            // velocities[i].x = x;
        }
    }
}

void GameSceneWidget::drawTextComponent(TextLabelComponent& text) {
    ImGui::SeparatorText("TextLabelComponent");
    std::string textStr = text.text;
    float x = text.position.x;
    float y = text.position.y;
    std::string assetId = text.assetId;
    SDL_Color color = text.color;
    bool isFixed = text.isFixed;
    bool isNested = text.isNested;

}

} 