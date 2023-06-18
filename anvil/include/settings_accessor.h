#pragma once
#include <string>

namespace anvil {
	class SettingsAccessor
	{
	public:
		virtual void load() = 0;
		virtual std::string getString(std::string key) = 0;
		virtual int getInt(std::string key) = 0;
	};
}
