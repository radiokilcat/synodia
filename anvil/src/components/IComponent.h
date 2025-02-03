#pragma once

#include "game_objects/IGameObject.h"
#include "nlohmann/json.hpp"
#include <string>

namespace anvil {
	class IGameObject;

	class IComponent {
public:
	virtual ~IComponent() = default;
	virtual void init() = 0;
	virtual void update(Uint64 deltaTime) = 0;
	virtual void from_json(const nlohmann::json& j) = 0;
	virtual void to_json(const nlohmann::json& j) = 0;
	
	void setOwner(std::shared_ptr<IGameObject> owner) {
		owner_ = owner;
	}
	
protected:
	std::shared_ptr<IGameObject> owner_;
};
	
}

