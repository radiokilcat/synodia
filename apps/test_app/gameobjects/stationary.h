#pragma once

#include "anvil.h"

class Stationary: public anvil::IsoGameObject
{
public:
    Stationary(const anvil::GameObjectData* params);
    ~Stationary() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;
    void init() override;

    static bool registerWithFactory();

    Stationary();
    void from_json(const nlohmann::json& j);
    void to_json(nlohmann::json& j);
//    bool isIntersect(int x, int y, int w, int h);
private:
    bool debug = false;

};
