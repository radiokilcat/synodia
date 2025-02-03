#pragma once
#include <nlohmann/json.hpp>

class SerializableBase 
{
public:
	virtual void to_json(nlohmann::json &j) = 0;
	virtual void from_json(const nlohmann::json &j) = 0;
};