#pragma once

#include "IComponent.h"
#include "nlohmann/json.hpp"
#include "vector2d.h"

namespace anvil {

class Transform2DComponent: public IComponent {
public:	
    Transform2DComponent(const nlohmann::json& data) {
        x_ = data.value("x", 0.0f);
        y_ = data.value("y", 0.0f);
    }
	~Transform2DComponent() = default;
    float getX() const          { return x_; }
    float getY() const          { return y_; }
    void setX(float x)          { x_ = x; }
    void setY(float y)          { y_ =  y; }
    Vector2D getPosition() { return Vector2D(x_, y_); }
	void setPosition(const Vector2D& position);
	
	virtual void init() {}
	virtual void update(Uint64 deltaTime) {}
	virtual void from_json(const nlohmann::json& j) {}
	virtual void to_json(const nlohmann::json& j) {}

private:
    float x_, y_;
};
    
}
