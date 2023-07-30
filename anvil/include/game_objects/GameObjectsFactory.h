#pragma once

#include <functional>
#include <memory>


namespace anvil {

class BaseGameObject;

using FactoryFunction = std::function<std::unique_ptr<BaseGameObject>()>;

class GameObjectFactory {
public:
    GameObjectFactory(const GameObjectFactory&) = delete;
    GameObjectFactory& operator=(const GameObjectFactory&) = delete;

    void registerType(std::string type, FactoryFunction function);
    std::unique_ptr<BaseGameObject> createGameObject(std::string type);

    static GameObjectFactory& instance();

private:
    GameObjectFactory() {};
    std::unordered_map<std::string, FactoryFunction> factoryFunctions;
};

}
