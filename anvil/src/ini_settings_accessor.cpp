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
}