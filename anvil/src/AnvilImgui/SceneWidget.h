#pragma once

#include "ImguiSystem.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/HealthComponent.h"
#include "../components/ProjectileEmitterComponent.h"
#include "../components/KeyboardControlledComponent.h"
#include "../components/TextLabelComponent.h"
#include "../components/AnimationComponent.h"
#include "../ECS/ECS.h"


namespace anvil {
	
	class IWidget;
	
	class GameSceneWidget : public IWidget {
	public:
		GameSceneWidget(const std::unique_ptr<Registry>& registry) : registry_(registry) {};
		void draw() override;
        void propertyWindow(bool p_open);
		void listEntity(const Entity& entity);
        void componentsProperties(const Entity& entity);

        void drawTransformComponent(TransformComponent& transform);
        void drawSpriteComponent(SpriteComponent& sprite);
        void drawAnimationComponent(AnimationComponent& animation);
        void drawRigidBodyComponent(RigidBodyComponent& rigidBody);
        void drawHealthComponent(HealthComponent& health);
        void drawProjectileEmitterComponent(ProjectileEmitterComponent& emitter);
        void drawKeyboardControlledComponent(KeyboardControlledComponent& keyboard);
        void drawTextComponent(TextLabelComponent& text);

    private:
		const std::unique_ptr<Registry>& registry_;
		int m_openEntityId = -1;
		bool showProperties = true;
		bool showInspector = true;
		std::queue<std::function<void()>> callbacks;
		bool show_demo_window = false;
	};
}
