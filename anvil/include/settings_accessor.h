#pragma once
#include <string>

namespace anvil {
	class SettingsAccessor
	{
	public:
		virtual void load() = 0;
		virtual void save() = 0;
		virtual std::string getString(std::string key) = 0;
		virtual int getInt(std::string key) = 0;
		
		virtual void setString(std::string key, std::string value) = 0;
		virtual void setInt(std::string key, int value) = 0;
	};
}
