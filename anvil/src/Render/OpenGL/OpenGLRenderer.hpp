#include "../IRenderer.hpp"

namespace anvil {

class OpenGLRenderer : public IRenderer {
public:
    bool init(void* window, int width, int height) override;
    void clear() override;
    void present() override;
    void drawTexture(/* your params */) override;
    void setLogicalSize(int width, int height) override;
    std::shared_ptr<ITexture> loadTextureFromFile(const std::string& path);

private:
    // GL handles (VAO, shader program, etc.)
};

}
    