#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "../json_serializer/serializable_base.h"

namespace anvil {
	using json = nlohmann::json;
 	class JsonSerializer
	{
	private:
		nlohmann::json m_parsedData;
		std::string m_filePath;
	public:
		JsonSerializer(std::string filePath);
		void serialize(SerializableBase* obj);
		void deserialize(SerializableBase* buffer);
	};

}