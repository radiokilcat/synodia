//#include "gamestate.h"
//#include "menubutton.h"
//#include "application.h"

//namespace anvil {

//std::string MenuState::getID()
//{
//    return menuID_;
//}

//bool MenuState::onEnter()
//{
//    return false;
//    if (!TextureManager::instance()->loadTexture("res/button.png",
//                                                 "playbutton", Application::Instance()->getRenderer()->getRenderer()))
//    {
//        return false;
//    }
//    if (!TextureManager::instance()->loadTexture("res/exit.png",
//                                                 "exitbutton", Application::Instance()->getRenderer()->getRenderer()))
//    {
//        return false;
//    }
//    GameObject* button1 = new MenuButton(new LoaderParams(100, 100, 400, 100, "playbutton"), []() { menuToPlay(); });
//    GameObject* button2 = new MenuButton(new LoaderParams(100, 300, 400, 100, "exitbutton"), []() { exitFromMenu(); });
//    game_objects_.push_back(button1);
//    game_objects_.push_back(button2);
//    std::cout << "Enter Menu state" << std::endl;
//    return true;
//}

//void MenuState::render()
//{
//    for (auto it: game_objects_)
//    {
//        it->draw(Application::Instance()->getRenderer());
//    }
//}

//bool MenuState::onExit()
//{
//    for (auto it: game_objects_)
//    {
//        it->clean();
//    }
//    game_objects_.clear();
//    TextureManager::instance()->clearFromTextureMap("playbutton");
//    TextureManager::instance()->clearFromTextureMap("exitbutton");
//    std::cout << "Exit Menu state" << std::endl;
//    return true;
//}

//void MenuState::update()
//{
//    for (auto it: game_objects_)
//    {
//        it->update();
//    }
//}

//void MenuState::menuToPlay()
//{
//    std::cout << "play button clicked" << std::endl;
////    Application::Instance()->getStateMachine()->changeState(new PlayState);
//}

//void MenuState::exitFromMenu()
//{
//    std::cout << "exit button clicked" << std::endl;
//    Application::Instance()->quit();
//}

//}

