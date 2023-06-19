#pragma once
#include <memory>
#include "inipp.h"
#include "settings_accessor.h"

namespace anvil {
	class IniSettingsAccessor : public SettingsAccessor 
	{
	public: 
		IniSettingsAccessor(std::string path);
		void load() override;
		std::string getString(std::string key) override;
		int getInt(std::string key) override;

	private: 
		std::string m_path;
		std::unique_ptr<inipp::Ini<char>> m_pIniParser;
	};
}