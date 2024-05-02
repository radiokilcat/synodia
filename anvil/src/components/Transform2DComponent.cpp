#include "components/Transform2DComponent.h"


void anvil::Transform2DComponent::setPosition(const Vector2D& position) {
	x_ = position.x();
	y_ = position.y();
}
