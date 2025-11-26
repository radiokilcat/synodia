#pragma once

#include <string>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include "Logger/Logger.h"
#include "Render/IRenderer.hpp"

namespace anvil {

inline RendererType rendererTypeFromString(const std::string& s) {
    if (s == "SDL") return RendererType::SDL;
    if (s == "OpenGL") return RendererType::OpenGL;
    throw std::invalid_argument("Unknown renderer type: " + s);
}

inline std::string rendererTypeToString(RendererType type) {
    switch (type) {
        case RendererType::SDL:    return "SDL";
        case RendererType::OpenGL: return "OpenGL";
        default:                   return "Unknown";
    }
}

struct AppSettings {

    int screenWidth = 256;
    int screenHeight = 224;
    int screenScale = 3;
    int FPS = 60;

    std::string windowTitle = "Game";
    RendererType rendererType = RendererType::SDL;

    void validate() {
        assert(screenWidth > 0);
        assert(screenHeight > 0);
        assert(screenScale > 0);
    }

    bool loadFromIni(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return false;

        std::string line;
        bool inWindowSection = false;

        while (std::getline(file, line)) {
            size_t comment = line.find(';');
            if (comment != std::string::npos) line = line.substr(0, comment);

            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty()) continue;

            if (line[0] == '[' && line.back() == ']') {
                std::string section = line.substr(1, line.size() - 2);
                inWindowSection = (section == "Window");
                continue;
            }

            if (!inWindowSection) continue;

            size_t eq = line.find('=');
            if (eq == std::string::npos) continue;

            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);

            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            try {
                if (key == "ScreenWidth") screenWidth = std::stoi(value);
                else if (key == "ScreenHeight") screenHeight = std::stoi(value);
                else if (key == "ScreenScale") screenScale = std::stoi(value);
                else if (key == "FPS") FPS = std::stoi(value);
                else if (key == "WindowTitle") windowTitle = value;
                else if (key == "RendererType") rendererType = rendererTypeFromString(value);
            } catch (...) {
                Logger::Err("Failed to parse INI key: {} = {}", key, value);
                return false;
            }
        }

        validate();
        return true;
    }

    bool saveToIni(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) return false;

        file << "[Window]\n";
        file << "ScreenWidth = " << screenWidth << "\n";
        file << "ScreenHeight = " << screenHeight << "\n";
        file << "ScreenScale = " << screenScale << "\n";
        file << "FPS = " << FPS << "\n";
        file << "WindowTitle = " << windowTitle << "\n";
        file << "RendererType = " << rendererTypeToString(rendererType) << "\n";

        return true;
    }
};


}
