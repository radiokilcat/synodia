#include "game_objects/GameObjectsFactory.h"

#include "game_objects/gameobject.h"

namespace anvil {

void GameObjectFactory::registerType(std::string type, FactoryFunction function) {
    factoryFunctions[type] = function;
}

std::unique_ptr<BaseGameObject> GameObjectFactory::createGameObject(std::string type) {
    auto iter = factoryFunctions.find(type);
    if (iter != factoryFunctions.end()) {
        return iter->second();
    } else {
        std::cerr << "Error: unknown type " << type << std::endl;
        return nullptr;
    }
}

GameObjectFactory& GameObjectFactory::instance() {
    static GameObjectFactory instance;
    return instance;
}

}
