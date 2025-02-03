#include "Sprite2DComponent.h"

#include "Transform2DComponent.h"
#include <iostream>


namespace anvil {
	
	bool endsWith(const std::string& str, const std::string& suffix) {
		if (str.size() < suffix.size()) return false;
		return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}
	
	Direction getAnimationDirection(std::string animation) {
		if (endsWith(animation, "_top")) {
			return Direction::Up;
		}
		if (endsWith(animation, "_right")) {
			return Direction::Right;
		}
		if (endsWith(animation, "_left")) {
			return Direction::Left;
		}
		if (endsWith(animation, "_bottom")) {
			return Direction::Down;
		} 
	}
	
	Sprite2DComponent::Sprite2DComponent(const nlohmann::json& data) {
		if (data.contains("image")) {
			const auto& images = data["image"];
			textureId_ = data["image"];
		}
		if (data.contains("animations")) {
			for (const auto& [key, value]: data["animations"].items() ) {
				std::unordered_map<Direction, std::string> animation;
				for (const auto& anim: value.items()) {
					animation[getAnimationDirection(anim.value())] = anim.value();
				}
				animations_[key] = animation;
			}
		}
		else if (data.contains("states")) {
			for (const auto& [key, value]: data["states"].items() ) {
				states_[key] = value;
			}
		}
		
		frameWidth_ = data.value("width", 0.0);
		frameHeight_ = data.value("height", 0.0);
		scale = data.value("scale", 1.f);
	}

	void Sprite2DComponent::draw_sheet(std::shared_ptr<Renderer> renderer) {
		auto transform = owner_->getComponent<Transform2DComponent>();
		auto x = transform->getX() - frameWidth_ * 0.5;
		auto y = transform->getY() - frameHeight_ * 0.5;
		
		int row = currentFrame_ / framesPerRow_ + 1;
		int col = currentFrame_ % framesPerRow_;
		
		TextureManager::instance()->drawFrame(animations_[currentAnimation_.first][currentAnimation_.second], x, y, frameWidth_, frameHeight_, row, col,
											renderer->getRenderer());
		TextureManager::instance()->drawPoint(renderer->getRenderer(), transform->getX(), transform->getY());
	}

	void Sprite2DComponent::draw(float x, float y, std::shared_ptr<Renderer> renderer) const {
		TextureManager::instance()->drawFrame(textureId_, x, y, frameWidth_, frameHeight_, currentRow_, currentFrame_,
											renderer->getRenderer());
	}

	void Sprite2DComponent::drawScaled(float x, float y, std::shared_ptr<Renderer> renderer) {
		TextureManager::instance()->drawFrameScaled(textureId_, scale, x, y, frameWidth_, frameHeight_,
													currentRow_, currentFrame_,
													renderer->getRenderer());
	}

	void Sprite2DComponent::setState(std::string state) {
		textureId_ = states_[state];
	}

	void Sprite2DComponent::addAnimation(std::string name, std::unordered_map<Direction, std::string> animation) {
		animations_[name] = animation;
	}

	void Sprite2DComponent::setCurrentAnimation(const std::string& animation, Direction direction) {
		currentAnimation_ = {animation, direction};
	}

	void Sprite2DComponent::update(Uint64 deltaTime) {
		auto del = deltaTime / 1000.f;
		elapsedTime_ += del;

		if (elapsedTime_ >= frameDuration_) {
			currentFrame_ = (currentFrame_ + 1) % totalFrames_;
			elapsedTime_ = 0.0f;
		}
		
	}
}
