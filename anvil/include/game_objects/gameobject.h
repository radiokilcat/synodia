#pragma once

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>

#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "vector2d.h"
#include "renderer.h"
#include "IGameObject.h"


namespace anvil {

enum class Direction { Up, Down, Left, Right, Static };

class GameObject : public IGameObject
{
public:
    GameObject() {};
    ~GameObject() {};

    void draw(std::shared_ptr<Renderer> renderer) override;
    void drawScaled(std::shared_ptr<Renderer> renderer, float scale);
    void load();
    void update(Uint64 deltaTime) override;;
    void clean() override {};
    void baseInit();
    void sort_children();
    virtual void init() {};
    bool getOutline() const { return outline_; }
    void setOutline(bool outline) { outline_ = outline; }
    
    virtual int getLayer();
    virtual void from_json(const nlohmann::json &j);
    virtual void to_json(nlohmann::json &j);
    virtual bool isCollide(std::shared_ptr<IGameObject> other);
    

protected:
    int layer_;
    bool outline_ = true;
};

}
