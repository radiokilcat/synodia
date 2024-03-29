#include "stationary.h"

Stationary::Stationary()
: anvil::IsoGameObject()
{

}

void Stationary::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::TextureManager::instance()->draw(textureId_, position_.x(), position_.y(), width_, height_,
                                            renderer->getRenderer());
    if (debug)
    {
        int x1 = getX();
        int y1 = getY();

        int x2 = x1 + width_;
        int y2 = y1;

        int x3 = x1 + width_;
        int y3 = y1 + height_;

        int x4 = x1;
        int y4 = y1 + height_;

        anvil::TextureManager::instance()->drawQuadrilateral(renderer->getRenderer(), x1, y1,
                                                             x2, y2,
                                                             x3, y3,
                                                             x4, y4);
    }
}

void Stationary::from_json(nlohmann::json& j)
{
    GameObject::from_json(j);
}

void Stationary::to_json(nlohmann::json& j)
{
    GameObject::to_json(j);
}

void Stationary::update()
{
}

void Stationary::clean()
{
}

void Stationary::init()
{
    auto screenW = anvil::Application::Instance()->getScreenWidth();
    auto screenH = anvil::Application::Instance()->getScreenHeight();
    position_.setX(screenW / 2 - width_ / 2);
    position_.setY(screenH / 2 - height_ / 2);
}

bool Stationary::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("Stationary", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<Stationary>();
    });
    return true;
}

//bool Stationary::isIntersect(int x, int y, int w, int h) {
//    int fxl = getX();
//    int fxr = getX() + width_;
//    int fyb = getY() + height_;
//    int fyt = getY();

//    int sxl = x;
//    int sxr = x + w;
//    int syb = y + h;
//    int syt = y;

//    if (fxr < sxl || sxr < fxl)
//        return false; // No horizontal overlap

//    if (fyb < syt || syb < fyt)
//        return false; // No vertical overlap
//    return true;
//}

