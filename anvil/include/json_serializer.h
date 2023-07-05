#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
template<typename T>
class JsonSerializer 
{
private:
	nlohmann::json m_parsedData;
	std::string m_filePath;
public:
	JsonSerializer(std::string filePath);
	void serialize(T &obj);
	T deserialize();
};

template<typename T>
JsonSerializer<T>::JsonSerializer(std::string filePath) : m_filePath(filePath)
{
}

template<typename T>
void JsonSerializer<T>::serialize(T &object)
{
	json j = object;
	std::ofstream outf(m_filePath);
	outf << j << std::endl;

}


template<typename T>
T JsonSerializer<T>::deserialize()
{
	std::ifstream fs(m_filePath);
	json data = json::parse(fs);
	auto d = data.template get<T>();
	return d;
}