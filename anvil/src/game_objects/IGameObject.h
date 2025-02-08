#pragma once

#include <typeindex>

#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "renderer.h"
#include "components/IComponent.h"

namespace anvil {
class IOLDComponent;

class IGameObject
{
public:
    virtual ~IGameObject() = default;
    virtual void draw(std::shared_ptr<Renderer> renderer) = 0;
    virtual void update(Uint64 deltaTime) = 0;
    virtual void clean() = 0;
    virtual void init() = 0;
    virtual void baseInit() = 0;
    virtual int  getLayer() = 0;
    
    virtual void addChild(std::shared_ptr<IGameObject> child) {
        m_children.push_back(child); 
    }
    
    virtual void removeChild(std::shared_ptr<IGameObject> game_object) {
        auto it = std::find(m_children.begin(), m_children.end(), game_object);
        if (it != m_children.end()) {
            m_children.erase(it);
        }
    }
    
    virtual const std::vector<std::shared_ptr<IGameObject>>& getChildren() {
        return m_children;
    };
    
    virtual void from_json(const nlohmann::json&) = 0;
    virtual void to_json(nlohmann::json&) = 0;
    virtual bool isCollide(std::shared_ptr<IGameObject>) = 0;

    template <typename T>
    void addComponent(std::shared_ptr<T> component) {
        components_[typeid(T)] = component;
    }

    template <typename T>
    void removeComponent() {
        components_.erase(typeid(T));
    }

    template <typename T>
    std::shared_ptr<T> getComponent() {
        auto it = components_.find(typeid(T));
        if (it != components_.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    const std::string& getId() const { return id_; };
    void setId(const std::string& id) { id_ = id; };
    
    bool operator==(const IGameObject& other) {
        return id_ == other.id_;
    }

    std::shared_ptr<IGameObject> findChild(const std::string& id) const {
        for (const auto& child : m_children) {
            if (child->getId() == id) {
                return child;
            }
            auto foundChild = child->findChild(id);
            if (foundChild) {
                return foundChild;
            }
        }
        return nullptr;
    }
    
protected:
    std::string id_;
    std::unordered_map<std::type_index, std::shared_ptr<IOLDComponent>> components_;
    std::vector<std::shared_ptr<anvil::IGameObject>> m_children;
};

}
