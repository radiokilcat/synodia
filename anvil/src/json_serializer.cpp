#include "json_serializer/json_serializer.h"

namespace anvil {
	JsonSerializer::JsonSerializer(std::string filePath) : m_filePath(filePath)
	{
	}

	void JsonSerializer::serialize(SerializableBase* object)
	{
		json j;
		object->to_json(j);
        std::ofstream outf(m_filePath);
        outf << j.dump(4) << std::endl;
	}

	void JsonSerializer::deserialize(SerializableBase* buffer)
	{
		std::ifstream fs(m_filePath);
		json data = json::parse(fs);
		buffer->from_json(data);
	}
}
