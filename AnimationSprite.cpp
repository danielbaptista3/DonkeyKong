#include "pch.h"
#include "AnimationSprite.h"

AnimationSprite::AnimationSprite(
	sf::Time frameTime,
	bool paused,
	bool looped
) :
	spriteAnimation(NULL),
	spriteFrameTime(frameTime),
	spriteCurrentFrame(0),
	spriteIsStopped(paused),
	spriteIsLooped(looped),
	spriteTexture(NULL)
{}

void AnimationSprite::setAnimation(const Animation& animation) {
	spriteAnimation = &animation;
	spriteTexture = spriteAnimation->getSpriteSheet();
	spriteCurrentFrame = 0;
	setFrame(spriteCurrentFrame);
}

void AnimationSprite::setFrameTime(sf::Time time) {
	spriteFrameTime = time;
}

void AnimationSprite::play() {
	spriteIsStopped = false;
}

void AnimationSprite::play(const Animation& animation) {
	if (getAnimation() != &animation) {
		setAnimation(animation);
	}

	play();
}

void AnimationSprite::pause() {
	spriteIsStopped = true;
}

void AnimationSprite::stop() {
	spriteIsStopped = true;
	spriteCurrentFrame = 0;
	setFrame(spriteCurrentFrame);
}

void AnimationSprite::setLooped(bool looped) {
	spriteIsLooped = looped;
}

void AnimationSprite::setColor(const sf::Color& color) {
	
	spriteVertices[0].color = color;
	spriteVertices[1].color = color;
	spriteVertices[2].color = color;
	spriteVertices[3].color = color;
}

const Animation* AnimationSprite::getAnimation() const {
	return spriteAnimation;
}

sf::FloatRect AnimationSprite::getLocalBounds() const {
	sf::IntRect rect = spriteAnimation->getFrame(spriteCurrentFrame);

	float width = static_cast<float>(std::abs(rect.width));
	float height = static_cast<float>(std::abs(rect.height));

	return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimationSprite::getGlobalBounds() const {
	return getTransform().transformRect(getLocalBounds());
}

bool AnimationSprite::isLooped() const {
	return spriteIsLooped;
}

bool AnimationSprite::isPlaying() const {
	return !spriteIsStopped;
}

sf::Time AnimationSprite::getFrameTime() const {
	return spriteFrameTime;
}

void AnimationSprite::setFrame(std::size_t newFrame, bool resetTime) {
	if (spriteAnimation) {
		
		sf::IntRect rect = spriteAnimation->getFrame(newFrame);

		spriteVertices[0].position = sf::Vector2f(0.f, 0.f);
		spriteVertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
		spriteVertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
		spriteVertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

		float left = static_cast<float>(rect.left) + 0.0001f;
		float right = left + static_cast<float>(rect.width);
		float top = static_cast<float>(rect.top);
		float bottom = top + static_cast<float>(rect.height);

		spriteVertices[0].texCoords = sf::Vector2f(left, top);
		spriteVertices[1].texCoords = sf::Vector2f(left, bottom);
		spriteVertices[2].texCoords = sf::Vector2f(right, bottom);
		spriteVertices[3].texCoords = sf::Vector2f(right, top);
	}

	if (resetTime) {
		spriteCurrentTime = sf::Time::Zero;
	}
}

void AnimationSprite::update(sf::Time deltaTime) {
	if (!spriteIsStopped && spriteAnimation) {
		spriteCurrentTime += deltaTime;

		if (spriteCurrentTime >= spriteFrameTime) {
			spriteCurrentTime = sf::microseconds(spriteCurrentTime.asMicroseconds() % spriteFrameTime.asMicroseconds());

			if (spriteCurrentFrame + 1 < spriteAnimation->getSize()) {
				spriteCurrentFrame++;
			}
			else {
				spriteCurrentFrame = 0; 

				if (!spriteIsLooped) {
					spriteIsStopped = true;
				}

			}

			setFrame(spriteCurrentFrame, false);
		}
	}
}

void AnimationSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (spriteAnimation && spriteTexture) {
		states.transform *= getTransform();
		states.texture = spriteTexture;
		target.draw(spriteVertices, 4, sf::Quads, states);
	}
}
