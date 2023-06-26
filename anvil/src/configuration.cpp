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

	void Configuration::save(GameSettings settings)
	{
		m_pConfigReader->setInt("ScreenWidth", settings.screenWidth);
		m_pConfigReader->setInt("ScreenHeight", settings.screenHeight);
		m_pConfigReader->setInt("ScreenScale", settings.screenScale);
		m_pConfigReader->setString("WindowTitle", settings.windowTitle);
		m_pConfigReader->save();
	}
}