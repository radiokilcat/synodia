#include <memory>
#include "ImguiSystem.h"
#include "SDLImguiSystem.h"
#include "OpenGLImguiSystem.h"

namespace anvil {

    static std::shared_ptr<ImguiSystem> createImGui(RendererType type) {
        switch (type) {
            case RendererType::SDL:
                return std::make_shared<SDLImguiSystem>();
            case RendererType::OpenGL:
                return std::make_shared<OpenGLImguiSystem>();
            default:
                return nullptr;
        }
    }

}