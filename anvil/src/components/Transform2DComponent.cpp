#include "components/Transform2DComponent.h"


void anvil::Transform2DComponent::setPosition(const Vector2D& position) {
	x_ = position.x();
	y_ = position.y();
}

void anvil::Transform2DComponent::setPivot(const Vector2D& pivot) {
	pivot_ = pivot;
}

float anvil::Transform2DComponent::getWidth() const {
	return width_;
}

float anvil::Transform2DComponent::getHeight() const {
	return height_;
}

anvil::Vector2D anvil::Transform2DComponent::getPivot() const {
	return pivot_;
}
