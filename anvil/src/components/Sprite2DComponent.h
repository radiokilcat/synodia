#pragma once

#include "IComponent.h"
#include "nlohmann/json.hpp"
#include "texturemanager.h"
#include "fmt/format.h"
#include "game_objects/gameobject.h"

namespace anvil {
	
class Sprite2DComponent : public IOLDComponent {
public:
	Sprite2DComponent(const nlohmann::json& data);
	~Sprite2DComponent() = default;

	const std::string& getTextureId() { return textureId_; }
	int getCurrentFrame() const { return currentFrame_; }
	void setCurrentFrame(int frame) { currentFrame_ = frame; }
	int getCurrentRow() const { return currentRow_; }
	void setCurrentRow(int row) { currentRow_ = row; }
	void setCurrentSheet(const std::string& sheet) { textureId_ = sheet; }
	void setCurrentAnimation(const std::string& animation, Direction direction); 

	void draw(float x, float y, std::shared_ptr<Renderer> renderer) const;
	void drawScaled(float x, float y, std::shared_ptr<Renderer> renderer);
	void draw_sheet(std::shared_ptr<Renderer> renderer);

	virtual void init() {};
	void update(Uint64 deltaTime) override;
	virtual void from_json(const nlohmann::json& j) {};
	virtual void to_json(const nlohmann::json& j) {};
		
	[[nodiscard]] float width() const { return frameWidth_; }
	[[nodiscard]] float height() const { return frameHeight_; }

	void setWidth(float width) { frameWidth_ = width; }
	void setHeight(float height) { frameHeight_ = height; }
	void setState(std::string state);
	
private:
	void addAnimation(std::string name, std::unordered_map<anvil::Direction, std::string> animation);
	std::string textureId_;
	std::unordered_map<std::string, std::string> textures_;
	std::unordered_map<std::string, std::unordered_map<Direction, std::string>> animations_;
	std::pair<std::string, Direction> currentAnimation_;

	int frameWidth_;
	int frameHeight_;
	int framesPerRow_ = 4;
	int totalFrames_ = 16;
	float frameDuration_ = 0.05f;
	int currentFrame_ = 0;
	int currentRow_ = 1;
	float elapsedTime_ = 0.f;
	float scale;
	std::unordered_map<std::string, std::string> states_;
};

}
