#include <memory>
#include "ImguiSystem.h"
#include "SDLImguiSystem.h"

namespace anvil {

    static std::shared_ptr<ImguiSystem> createImGui(RendererType type) {
        switch (type) {
            case RendererType::SDL: 
                return std::make_shared<SDLImguiSystem>();
            case RendererType::OpenGL: 
                // return std::make_unique<ImguiSystemOpenGL>();
                Logger::Err("OpenGL ImguiSystem is not implemented yet.");
            default:
                return nullptr;
        }
        return nullptr;
    }

}