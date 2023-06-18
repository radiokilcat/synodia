#include <iostream>
#include "anvil.h"

int main(int argc, char *argv[])
{
    anvil::Configuration configuration("settings.ini");
    auto settings = configuration.load();
    settings.validate();
    auto app = anvil::Application::create(settings);
    app->run();

    return 0;
}
