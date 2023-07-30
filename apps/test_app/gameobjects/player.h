#pragma once

#include "anvil.h"
#include "speech.h"


class Player: public anvil::IsoGameObject
{
public:
    Player();
    ~Player() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

    static bool registerWithFactory();
private:
    std::string _quotes[4] = { 
        std::string("Whoa!\nTHE POWER\nOF ENGINE"),
        std::string("Gotta win!"),
        std::string("take that!"),
        std::string("nice one!"),
    };
    std::unique_ptr<Speech> _speech;
};


