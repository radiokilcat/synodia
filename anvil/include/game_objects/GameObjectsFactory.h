#pragma once

#include <functional>
#include <memory>
#include <string>

namespace anvil {

class IGameObject;

using FactoryFunction = std::function<std::unique_ptr<IGameObject>()>;

class GameObjectFactory {
public:
    GameObjectFactory(const GameObjectFactory&) = delete;
    GameObjectFactory& operator=(const GameObjectFactory&) = delete;

    void registerType(const std::string& type, FactoryFunction function);
    std::unique_ptr<IGameObject> createGameObject(const std::string& type);

    static GameObjectFactory& instance();

private:
    GameObjectFactory() {};
    std::unordered_map<std::string, FactoryFunction> factoryFunctions;
};

}
