#include <fstream>
#include "ini_settings_accessor.h"
#include "inipp.h"

namespace anvil
{
	IniSettingsAccessor::IniSettingsAccessor(std::string path) 
		: m_path(path), m_pIniParser(std::make_unique<inipp::Ini<char>>())
	{
	}

    void IniSettingsAccessor::load() 
	{
		std::ifstream is(m_path);
		m_pIniParser->parse(is);
	}

	void IniSettingsAccessor::save()
	{
		std::ofstream of(m_path);
		m_pIniParser->generate(std::cout);
		m_pIniParser->generate(of);
	}

	std::string IniSettingsAccessor::getString(std::string key)
	{
		std::string result;
		inipp::get_value(m_pIniParser->sections["Window"], key, result);
		return result;
	}

	int IniSettingsAccessor::getInt(std::string key)
	{
		int result;
		inipp::get_value(m_pIniParser->sections["Window"], key, result);
		return result;
	}

	void IniSettingsAccessor::setString(std::string key, std::string value)
	{
		m_pIniParser->sections["Window"][key] = value;
	}

	void IniSettingsAccessor::setInt(std::string key, int value)
	{
		m_pIniParser->sections["Window"][key] = std::to_string(value);
	}
}