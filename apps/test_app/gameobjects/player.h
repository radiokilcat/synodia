#pragma once

#include "anvil.h"
#include "speech.h"



class Player: public anvil::IsoGameObject
{
public:
    Player();

    float getX();
    float getY();

    float getWidth();
    float getHeight();
//    int getScreenX();
//    int getScreenY();

    ~Player() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

    void from_json(nlohmann::json& j) override;
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

    std::unique_ptr<Speech> _speech;
    bool _continuedMove = false;
    bool _inverseMove = false;
    bool _blockInteractions = false;
};


