#include "pch.h"
#include "Animation.h"

Animation::Animation() : animationTexture(NULL) {}

void Animation::addFrame(sf::IntRect rect) {
	animationFrames.push_back(rect);
}

void Animation::setSpriteSheet(const sf::Texture& texture) {
	animationTexture = &texture;
}

const sf::Texture* Animation::getSpriteSheet() const {
	return animationTexture;
}

std::size_t Animation::getSize() const {
	return animationFrames.size();
}

const sf::IntRect& Animation::getFrame(std::size_t n) const {
	return animationFrames[n];
}