#pragma once
#include <memory>
#include "inipp.h"
#include "settings_accessor.h"

namespace anvil {
	class IniSettingsAccessor : public SettingsAccessor 
	{
	public: 
		IniSettingsAccessor(std::string path);
		~IniSettingsAccessor();
		void load() override;
		void save() override;
		std::string getString(std::string key) override;
		int getInt(std::string key) override;
		void setString(std::string key, std::string value) override;
		void setInt(std::string key, int value) override;
	private: 
		std::string m_path;
		std::unique_ptr<inipp::Ini<char>> m_pIniParser;
	};
}