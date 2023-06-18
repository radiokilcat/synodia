#include "configuration.h"
#include "ini_settings_accessor.h"

namespace anvil
{
	Configuration::Configuration(std::string path) 
	{
		m_pConfigReader = std::make_unique<IniSettingsAccessor>(path);
	}

	GameSettings Configuration::load()
	{
		m_pConfigReader->load();
		GameSettings settings {
			.screenWidth = m_pConfigReader->getInt("ScreenWidth"),
			.screenHeight = m_pConfigReader->getInt("ScreenHeight"),
			.screenScale = m_pConfigReader->getInt("ScreenScale"),
			.windowTitle = m_pConfigReader->getString("WindowTitle"),
		};
		return settings;
	}
}