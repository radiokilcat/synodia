#pragma once

#include "IComponent.h"
#include "nlohmann/json.hpp"
#include "vector2d.h"

namespace anvil {

class Transform2DComponent: public IOLDComponent {
public:	
    Transform2DComponent(const nlohmann::json& data) {
        x_ = data.value("x", 0.0f);
        y_ = data.value("y", 0.0f);
        width_ = data.value("width", 0.0f);
        height_ = data.value("height", 0.0f);
    	pivot_ = Vector2D{0.5, 0.5};
    }
	~Transform2DComponent() = default;
    [[nodiscard]] float getX() const       { return x_; }
    [[nodiscard]] float getY() const       { return y_; }
    [[nodiscard]] Vector2D getPivot() const;
    [[nodiscard]] Vector2D getPosition() { return Vector2D(x_, y_); }
    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;

    void setX(const float& x)       { x_ = x; }
    void setY(const float& y)       { y_ = y; }
    void setWidth(const float& width)       { width_ = width; }
    void setHeight(const float& height)       { height_ = height; }
	void setPosition(const Vector2D& position);
	void setPivot(const Vector2D& pivot);
	
	virtual void init() {}
	virtual void update(Uint64 deltaTime) {}
	virtual void from_json(const nlohmann::json& j) {}
	virtual void to_json(const nlohmann::json& j) {}

private:
    float x_, y_;
    float width_, height_;
    Vector2D pivot_;
};
    
}
