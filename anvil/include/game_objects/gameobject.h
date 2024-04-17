#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "vector2d.h"
#include "renderer.h"
#include "texturemanager.h"
#include "inputhandler.h"
#include "application.h"
#include "IGameObject.h"


namespace anvil {

enum class Direction { Up, Down, Left, Right, Static };
    

class GameObjectData
{
public:
    GameObjectData(float x, float y, float width, float height, std::string id)
        : x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
        , texture_id_(id)
        {};
    float getX() const          { return x_; }
    float getY() const          { return y_; }
    float getWidth() const      { return width_; }
    float getHeight() const     { return height_; }
    std::string getId() const { return texture_id_; }

private:
    float x_, y_;
    float width_, height_;
    std::string texture_id_;
};
    
class GameObject : public IGameObject
{
public:
    GameObject();

    virtual void draw(std::shared_ptr<Renderer> renderer) override;
    virtual void drawScaled(std::shared_ptr<Renderer> renderer, float scale);
    virtual void update() override {};
    virtual void clean() override {};
    virtual ~GameObject() {};
    virtual void load(const anvil::GameObjectData* params);
    virtual void addChild(std::shared_ptr<IGameObject> child);
    virtual const std::vector<std::shared_ptr<IGameObject>>& getChildren();
    virtual int getZOrder();
    virtual void init();

    virtual void from_json(const nlohmann::json &j);
    virtual void to_json(nlohmann::json &j);

    bool operator==(const GameObject& g);

    Vector2D getVelocity() { return velocity_;  };
    void setVelocity(Vector2D velocity) { velocity_ = velocity; }

    const std::string& getId() const;
    const std::string& getTextureId() const;
    int getCurrentFrame() const;
    int getCurrentRow() const;
    const Vector2D& getPosition() const;
    const Vector2D& getAcceleration() const;
    float getWidth() const;
    float getHeight() const;
    void setWidth(float width);
    void setHeight(float height);

    void setX(float x);
    void setY(float y);
    
protected:
    std::string id_;
    std::string textureId_;
    int currentFrame_ = 0;
    int currentRow_ = 0;
    Vector2D position_;
    Vector2D velocity_;
    Vector2D acceleration_;
    float width_, height_;
    int zOrder_;

    std::vector<std::shared_ptr<anvil::IGameObject>> m_childs;
    
};

}
