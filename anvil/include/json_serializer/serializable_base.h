#pragma once
#include <nlohmann/json.hpp>

template<typename T>
class SerializableBase 
{
public:
	virtual void to_json(nlohmann::json &j, T &object) = 0;
	virtual void from_json(nlohmann::json &j, T &object) = 0;
};