#pragma once

#include "anvil.h"
#include "speech.h"


namespace anvil {
    class MovementIsoComponent;
    class Sprite2DComponent;
}

class Player: public anvil::GameObject
{
public:
    Player();
    ~Player() {};
    
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update(Uint64 deltaTime) override;
    void clean() override;
    void init() override;

    void from_json(const nlohmann::json& j) override;
    void to_json(nlohmann::json& j) override;


    static bool registerWithFactory();

    bool continuedMove() const;
    anvil::Direction moveDirection();
    void setContinuedMove(bool newContinuedMove);
    void setInverseMove(bool inverseMove);
    void blockInteractions();
private:
    std::string _quotes[4] = { 
        std::string("Whoa!\nTHE POWER\nOF ENGINE"),
        std::string("Gotta win!"),
        std::string("take that!"),
        std::string("nice one!"),
    };

    // std::unique_ptr<Speech> _speech;
    bool _continuedMove = false;
    bool _inverseMove = false;
    bool _blockInteractions = false;
    bool isMoving = false;
    std::shared_ptr<anvil::Sprite2DComponent> sprite_;
    std::shared_ptr<anvil::Transform2DComponent> transform_;
    std::shared_ptr<anvil::MovementIsoComponent> movement_;
    anvil::Direction lastDirection_ = anvil::Direction::Down;
};


