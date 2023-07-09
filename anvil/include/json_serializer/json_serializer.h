#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "serializable_base.h"
using json = nlohmann::json;
template<typename TSerializer, typename T>
class JsonSerializer 
{
private:
	nlohmann::json m_parsedData;
	std::string m_filePath;
	TSerializer m_serializer;
public:
	JsonSerializer(std::string filePath, TSerializer serializer);
	void serialize(T &obj);
	T deserialize();
};

template<typename TSerializer, typename T>
JsonSerializer<TSerializer, T>::JsonSerializer(std::string filePath, TSerializer serializer) : m_filePath(filePath), m_serializer(serializer)
{
}

template<typename TSerializer, typename T>
void JsonSerializer<TSerializer, T>::serialize(T &obj)
{
	json j;
	std::ofstream outf(m_filePath);
	m_serializer.to_json(j, obj);
	outf << j << std::endl;
}


template<typename TSerializer, typename T>
T JsonSerializer<TSerializer, T>::deserialize()
{
	std::ifstream fs(m_filePath);
	json data = json::parse(fs);
	T result;
	m_serializer.from_json(data, result);
	return result;
}