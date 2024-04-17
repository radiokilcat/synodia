#pragma once

#include "game_objects/gameobject.h"
#include "game_objects/textlabel.h"

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "vector2d.h"
#include "renderer.h"
#include "texturemanager.h"
#include "logger.h"
#include <fmt/format.h>

namespace anvil {

GameObject::GameObject() :
    position_(0, 0),
    velocity_(.0f, .0f),
    acceleration_(0,0),
    id_("") {
    currentFrame_ = 0;
    currentRow_ = 0;
    height_ = 0.f;
    width_ = 0.f;
}

void GameObject::draw(std::shared_ptr<Renderer> renderer)
{
    TextureManager::instance()->drawFrame(id_, (float)position_.x(), (float)position_.y(),
                                          (float)width_, (float)height_,
                                          currentRow_, currentFrame_,
                                          renderer->getRenderer());
}

void GameObject::drawScaled(std::shared_ptr<Renderer> renderer, float scale)
{
    TextureManager::instance()->drawFrameScaled(id_, scale,(float)position_.x(), (float)position_.y(),
                                                (float)width_, (float)height_,
                                                currentRow_, currentFrame_,
                                                renderer->getRenderer());
}

void GameObject::load(const anvil::GameObjectData* params)
{
    position_ = Vector2D{(float)params->getX(), (float)params->getY()};
    velocity_ = Vector2D{0,0};
    acceleration_ = Vector2D{0,0};
    width_ = params->getWidth();
    height_ = params->getHeight();
    id_ = params->getId();
    currentRow_ = 1;
    currentFrame_ = 1;
}

void GameObject::addChild(std::shared_ptr<IGameObject> child)
{
    m_childs.push_back(child);
    // TODO: Uncomment when GameScene::addChildObject fixed
    std::sort(m_childs.begin(), m_childs.end(), [](const std::shared_ptr<anvil::IGameObject>& a,
                                                   const std::shared_ptr<anvil::IGameObject>& b) {
        return a->getZOrder() < b->getZOrder();
    });
}


// void GameObject::removeChild(std::shared_ptr<IGameObject> child) {
//     auto it = std::remove(m_childs.begin(), m_childs.end(), child);
//     m_childs.erase(it, m_childs.end());
// }

const std::vector<std::shared_ptr<IGameObject>>& GameObject::getChildren() {
    return m_childs;
}

int GameObject::getZOrder()
{
    return zOrder_;
}

void GameObject::init()
{
    // auto debugLabel = std::make_shared<TextLabel>(id_, anvil::Color{255, 0, 0});
    // debugLabel->setPosition(position_.x(), position_.y() - 15.f);
    // debugLabel->setSize(width_, 15.f);
    // addChild(debugLabel);
}

void GameObject::from_json(const nlohmann::json& j) {
    id_ = j.value("id", std::string("test1"));
    textureId_ = j.value("textureId", std::string("test"));
    currentFrame_ = j.value("currentFrame", 0);
    currentRow_ = j.value("currentRow", 0);
    position_ = j.value("position", Vector2D{0,0});
    velocity_ = j.value("velocity", Vector2D{0,0});
    acceleration_ = j.value("acceleration", Vector2D{0,0});
    width_ = j.value("width", 0.f);
    height_ = j.value("height", 0.f);
    zOrder_ = j.value("zOrder", 0);
}

void GameObject::to_json(nlohmann::json& j)  {
    j["id"] = id_;
    j["textureId"] = textureId_;
    j["currentFrame"] = currentFrame_;
    j["currentRow"] = currentRow_;
    j["position"] = position_;
    j["velocity"] = velocity_;
    j["acceleration"] = acceleration_;
    j["width"] = width_;
    j["height"] = height_;
    j["zOorder"] = zOrder_;
}

bool GameObject::operator==(const GameObject& g) {
    return id_ == g.id_;
}

    const std::string &GameObject::getId() const {
        return id_;
    }

    const std::string &GameObject::getTextureId() const {
        return textureId_;
    }

    int GameObject::getCurrentFrame() const {
        return currentFrame_;
    }

    int GameObject::getCurrentRow() const {
        return currentRow_;
    }

    const Vector2D& GameObject::getPosition() const {
        return position_;
    }

    const Vector2D& GameObject::getAcceleration() const {
        return acceleration_;
    }

    float GameObject::getWidth() const {
        return width_;
    }

    float GameObject::getHeight() const {
        return height_;
    }

    void GameObject::setWidth(float width) {
        width_ = width;
    }
    
    void GameObject::setHeight(float height) {
        height_ = height;
    }
    
    void GameObject::setX(float x) {
        position_.setX(x);
    }
    
    void GameObject::setY(float y) {
        position_.setY(y);
    }
}
