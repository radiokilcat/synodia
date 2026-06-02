#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../IRenderer.hpp"

namespace anvil {

class OpenGLRenderer : public IRenderer {
public:
	OpenGLRenderer();
	~OpenGLRenderer();
	bool init(void* window, int width, int height) override;
	void clear() override;
	void present() override;
	void drawTexture() override;
	void setLogicalSize(int width, int height) override;
	void renderTextureRotated(
		ITexture* texture,
		const FRect* srcRect,
		const FRect* dstRect,
		double angle,
		const FPoint* center,
		FlipMode flip
	) override;
	std::shared_ptr<ITexture> loadTextureFromFile(const std::string& path) override;
	std::shared_ptr<ITexture> createTextTexture(const std::string& text, IFont* font, Color color) override;
	void fillRect(const FRect& rect, Color color) override;
	RendererType getType() const override { return RendererType::OpenGL; }

private:
	SDL_Window* m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	GLuint m_shaderProgram = 0;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	GLint m_projectionLoc = -1;
	GLint m_colorLoc = -1;
	GLint m_useTextureLoc = -1;
	GLint m_samplerLoc = -1;
	glm::mat4 m_projection = glm::mat4(1.0f);
	int m_logicalWidth = 0;
	int m_logicalHeight = 0;

	void setup2DProjection(int width, int height);
	bool initGL(int width, int height);
	bool initGLResources();
	bool createShaderProgram();
	bool createQuadBuffers();
	void destroyGLResources();
};
}
