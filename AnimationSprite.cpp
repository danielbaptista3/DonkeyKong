#include "pch.h"
#include "AnimationSprite.h"

AnimationSprite::AnimationSprite(
	sf::Time frameTime,
	bool paused,
	bool looped
) :
	mAnimation(NULL),
	mFrameTime(frameTime),
	mCurrentFrame(0),
	mIsPaused(paused),
	mIsLooped(looped),
	mTexture(NULL)
{}

void AnimationSprite::setAnimation(const Animation& animation) {
	mAnimation = &animation;
	mTexture = mAnimation->getSpriteSheet();
	mCurrentFrame = 0;
	setFrame(mCurrentFrame);
}

void AnimationSprite::setFrameTime(sf::Time time) {
	mFrameTime = time;
}

void AnimationSprite::play() {
	mIsPaused = false;
}

void AnimationSprite::play(const Animation& animation) {
	if (getAnimation() != &animation) {
		setAnimation(animation);
	}

	play();
}

void AnimationSprite::pause() {
	mIsPaused = true;
}

void AnimationSprite::stop() {
	mIsPaused = true;
	mCurrentFrame = 0;
	setFrame(mCurrentFrame);
}

void AnimationSprite::setLooped(bool looped) {
	mIsLooped = looped;
}

void AnimationSprite::setColor(const sf::Color& color) {
	
	mVertices[0].color = color;
	mVertices[1].color = color;
	mVertices[2].color = color;
	mVertices[3].color = color;
}

const Animation* AnimationSprite::getAnimation() const {
	return mAnimation;
}

sf::FloatRect AnimationSprite::getLocalBounds() const {
	sf::IntRect rect = mAnimation->getFrame(mCurrentFrame);

	float width = static_cast<float>(std::abs(rect.width));
	float height = static_cast<float>(std::abs(rect.height));

	return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimationSprite::getGlobalBounds() const {
	return getTransform().transformRect(getLocalBounds());
}

bool AnimationSprite::isLooped() const {
	return mIsLooped;
}

bool AnimationSprite::isPlaying() const {
	return !mIsPaused;
}

sf::Time AnimationSprite::getFrameTime() const {
	return mFrameTime;
}

void AnimationSprite::setFrame(std::size_t newFrame, bool resetTime) {
	if (mAnimation) {
		
		sf::IntRect rect = mAnimation->getFrame(newFrame);

		mVertices[0].position = sf::Vector2f(0.f, 0.f);
		mVertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
		mVertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
		mVertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

		float left = static_cast<float>(rect.left) + 0.0001f;
		float right = left + static_cast<float>(rect.width);
		float top = static_cast<float>(rect.top);
		float bottom = top + static_cast<float>(rect.height);

		mVertices[0].texCoords = sf::Vector2f(left, top);
		mVertices[1].texCoords = sf::Vector2f(left, bottom);
		mVertices[2].texCoords = sf::Vector2f(right, bottom);
		mVertices[3].texCoords = sf::Vector2f(right, top);
	}

	if (resetTime) {
		mCurrentTime = sf::Time::Zero;
	}
}

void AnimationSprite::update(sf::Time deltaTime) {
	if (!mIsPaused && mAnimation) {
		mCurrentTime += deltaTime;

		if (mCurrentTime >= mFrameTime) {
			mCurrentTime = sf::microseconds(mCurrentTime.asMicroseconds() % mFrameTime.asMicroseconds());

			if (mCurrentFrame + 1 < mAnimation->getSize()) {
				mCurrentFrame++;
			}
			else {
				mCurrentFrame = 0; 

				if (!mIsLooped) {
					mIsPaused = true;
				}

			}

			setFrame(mCurrentFrame, false);
		}
	}
}

void AnimationSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (mAnimation && mTexture) {
		states.transform *= getTransform();
		states.texture = mTexture;
		target.draw(mVertices, 4, sf::Quads, states);
	}
}
