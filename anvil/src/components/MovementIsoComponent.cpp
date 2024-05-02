#include "components/MovementIsoComponent.h"
#include "components/Transform2DComponent.h"

namespace anvil {
	void MovementIsoComponent::update(Uint64 deltaTime) {
		auto transform = owner_->getComponent<Transform2DComponent>();
		if (transform) {
			auto position = transform->getPosition();
			velocity_ += acceleration_;
			auto upd_position = position + velocity_;
			transform->setPosition(upd_position);
		}
	}

	Direction MovementIsoComponent::moveDirection() {
		if (velocity_.x() > 0 && velocity_.y() > 0) { return Direction::Down; }
		if (velocity_.x() < 0 && velocity_.y() > 0) { return Direction::Left; }
		if (velocity_.x() > 0 && velocity_.y() < 0) { return Direction::Right; }
		if (velocity_.x() < 0 && velocity_.y() < 0) { return Direction::Up; }
		if (isNearlyZero(velocity_.x()) && isNearlyZero(velocity_.x())) { return Direction::Static; }
		return Direction::Static;
	}
	
	void MovementIsoComponent::setVelocityTowardTarget(const Vector2D& targetPosition, float speed) {
		auto transform = owner_->getComponent<Transform2DComponent>();
		auto position = transform->getPosition();
		Vector2D direction = targetPosition - position;
		direction.normalize();
		
		float isoVelocityX = direction.x() - direction.y();
		float isoVelocityY = (direction.x() + direction.y()) / 2;
		
		isoVelocityX *= speed;
		isoVelocityY *= speed;
		
		velocity_.setX(isoVelocityX);
		velocity_.setY(isoVelocityY);
	}
	
	void MovementIsoComponent::setVelocityTowardObject(const std::shared_ptr<IGameObject> other, float speed) {
		auto transform = owner_->getComponent<Transform2DComponent>();
		auto position = transform->getPosition();
		auto otherTransform = other->getComponent<Transform2DComponent>();
		auto otherPos = Vector2D{otherTransform->getX(), otherTransform->getY()};
		Vector2D direction = otherPos - position;
		direction.normalize();
		
		float isoVelocityX = direction.x() - direction.y();
		float isoVelocityY = (direction.x() + direction.y()) / 2;
		
		isoVelocityX *= speed;
		isoVelocityY *= speed;
		
		velocity_.setX(isoVelocityX);
		velocity_.setY(isoVelocityY);
	}
	
	bool MovementIsoComponent::isNearlyZero(float value, float epsilon)
	{
		return std::fabs(value) < epsilon;
	}

	Vector2D MovementIsoComponent::toIso(const Vector2D& pos) {
		float isoX = pos.x() + pos.y();
		float isoY = (pos.x() - pos.y()) / 2;
		return {isoX, isoY};
	}

	void MovementIsoComponent::setVelocity(const Vector2D& vel) {
		velocity_ = toIso(vel);
	}
}
