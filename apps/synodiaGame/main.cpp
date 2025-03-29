#include <filesystem>

#include "anvil.h"


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::filesystem::current_path(anvil::getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets";

    anvil::AppSettings settings;
    if (!settings.loadFromIni("settings.ini")) {
        anvil::Logger::Err("Failed to load game settings");
    }

    auto app = anvil::Application::Instance();
    app->init(settings);
    app->run();

    return 0;
}
