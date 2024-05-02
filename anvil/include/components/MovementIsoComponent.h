#pragma once

#include "IComponent.h"
#include "vector2d.h"
#include "game_objects/gameobject.h"


namespace anvil {
	class Transform2DComponent;
	
	class MovementIsoComponent : public IComponent {
	public:
		MovementIsoComponent(const nlohmann::json& data): velocity_(0.0f, 0.0f), acceleration_(0.0f, 0.0f) { };

		MovementIsoComponent() : velocity_(0.0f, 0.0f), acceleration_(0.0f, 0.0f) {}

		void update(Uint64 deltaTime);

		Direction moveDirection();
		void setVelocityTowardTarget(const Vector2D& targetPosition, float speed);
		void setVelocityTowardObject(std::shared_ptr<IGameObject> other, float speed);
		void setVelocityTowardObject(const IGameObject& other, float speed);
		bool isNearlyZero(float value, float epsilon = 1e-5f);
		Vector2D toIso(const Vector2D& pos);
		// void update(float deltaTime) {
			// velocity += acceleration * deltaTime;

			// auto position = owner->getPosition();
			// auto x = owner_->getComponent<Transform2DComponent>()->getX();
			// entity->setPosition(position + velocity * deltaTime);
		// }

		void setVelocity(const Vector2D & vel);
		Vector2D getVelocity() const { return velocity_; }

		void setAcceleration(const Vector2D& accel) { acceleration_ = accel; }
		Vector2D getAcceleration() const { return acceleration_; }

		virtual void init() {};
		virtual void from_json(const nlohmann::json& j) {};
		virtual void to_json(const nlohmann::json& j) {};
		
	private:
		Vector2D velocity_;
		Vector2D acceleration_;
	};

}
