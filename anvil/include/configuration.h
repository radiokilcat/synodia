#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "application.h"
#include "settings_accessor.h"

namespace anvil
{
	class Configuration
	{
	public:
		Configuration(std::string path);
		GameSettings load();
	private:
		std::unique_ptr<SettingsAccessor> m_pConfigReader;
	};
}
