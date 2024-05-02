#pragma once

#include "anvil.h"


// class Speech : public anvil::GameObject
// {
// public:
//     enum Frames {
//         HIDDEN = 0, 
//         SHOWN = 1
//     };
//     Speech();
//     ~Speech() {};
//     void draw(std::shared_ptr<anvil::Renderer> renderer) override;
//     void update(Uint64 deltaTime) override;
//     void setPosition(float x, float y);
//     void show(int timeSec, std::string text);
//     bool isShown();
//     void clean() override;
//     void load();
//
//     void setTextSize(float size);
//     void setSize(float w, float h);
// private:
//     anvil::TextLabel* m_label = nullptr;
//     std::string m_text = "";
//     int shownAt = 0;
//     int showDuration = 0;
//     float m_scale = 1;
// };
