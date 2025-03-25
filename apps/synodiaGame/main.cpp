#include <iostream>
#include <filesystem>

#include "anvil.h"


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::filesystem::current_path(anvil::getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets";

    anvil::Configuration configuration("settings.ini");
    auto settings = configuration.load();
    settings.validate();
    auto app = anvil::Application::Instance();
    app->init(settings);
    app->run();

    return 0;
}
