#pragma once

#include "IComponent.h"
#include "Sprite2DComponent.h"
#include "Transform2DComponent.h"
#include "nlohmann/json.hpp"

namespace anvil {
    
class CollisionComponent: public IComponent {
public:
    CollisionComponent(float width, float height) : width_(width), height_(height) {}

	virtual void init() {};
	virtual void from_json(const nlohmann::json& j) {};
	virtual void to_json(const nlohmann::json& j) {};
    
	void update(Uint64 deltaTime) override {
		auto transform = owner_->getComponent<Transform2DComponent>();
		x_ = transform->getX();
		y_ = transform->getY();
	};
	
void drawOutline(std::shared_ptr<Renderer> renderer) {
		auto transform = owner_->getComponent<Transform2DComponent>();
        auto x = x_;
        auto y = y_;
		float x1 = x;
		float y1 = y;
		float x2 = x + width_;
		float y2 = y;
		float x3 = x + width_;
		float y3 = y + height_;
		float x4 = x;
		float y4 = y + height_;
		TextureManager::instance()->drawQuadrilateral(renderer->getRenderer(), x1, y1, x2, y2, x3, y3, x4, y4);
}
	
    bool checkCollision(const CollisionComponent& other) const {
        // AABB (Axis-Aligned Bounding Box) collision detection
        return (x_ < other.x_ + other.width_ &&
                x_ + width_ > other.x_ &&
                y_ < other.y_ + other.height_ &&
                y_ + height_ > other.y_);
    }

    void setPosition(float x, float y) {
        x_ = x;
        y_ = y;
    }

private:
    float x_ = 0.0f;
    float y_ = 0.0f;
    float width_;  
    float height_;
};
    
}