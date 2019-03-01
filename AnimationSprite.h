#pragma once

#include "Animation.h"

class AnimationSprite : public sf::Drawable, public sf::Transformable {
public:
	explicit AnimationSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);

	void update(sf::Time deltaTime);
	void setAnimation(const Animation& animation);
	void setFrameTime(sf::Time time);
	void play();
	void play(const Animation& animation);
	void pause();
	void stop();
	void setLooped(bool looped);
	void setColor(const sf::Color& color);
	const Animation* getAnimation() const;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	bool isLooped() const;
	bool isPlaying() const;
	sf::Time getFrameTime() const;
	void setFrame(std::size_t newFrame, bool resetTime = true);

private:
	const Animation* spriteAnimation;
	sf::Time spriteFrameTime;
	sf::Time spriteCurrentTime;
	std::size_t spriteCurrentFrame;
	bool spriteIsStopped;
	bool spriteIsLooped;
	const sf::Texture* spriteTexture;
	sf::Vertex spriteVertices[4];

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};