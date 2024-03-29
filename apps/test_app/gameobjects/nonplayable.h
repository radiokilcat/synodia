#pragma once

#include "anvil.h"

class NonPlayable: public anvil::IsoGameObject
{
public:
    NonPlayable(const anvil::LoaderParams* params);
    ~NonPlayable() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

    void from_json(const nlohmann::json& j) override;
    void to_json(nlohmann::json& j) override;

    static bool registerWithFactory();

    NonPlayable();
private:


};
