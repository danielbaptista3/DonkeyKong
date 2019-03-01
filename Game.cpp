#include <memory>

#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "Animation.h"
#include "AnimationSprite.h"
#include "GameHelper.h"

const float Game::PlayerSpeed = 150.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const std::string BlockTexturePath = "Media/Textures/Block.png";
const std::string LadderTexturePath = "Media/Textures/Echelle.PNG";
const std::string MarioSpriteSheetPath = "Media/Textures/mario_sprite.png";
const std::string StatisticsFontPath = "Media/Sansation.ttf";
const std::string CoinTexturePath = "Media/Textures/coin.png";
const std::string ScoreFontPath = "Media/Sansation.ttf";
const sf::Time jumpTime = sf::seconds(0.15f);

Game::Game() :
	mWindow(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Donkey Kong 1981", sf::Style::Close),
	mSpriteSheet(),
	mFont(),
	scoreFont(),
	scoreAnnouncementText(),
	scoreText(),
	mStatisticsText(),
	mStatisticsUpdateTime(),
	mStatisticsNumFrames(0),
	score(0),
	debug(true)
{
	mWindow.setFramerateLimit(160);
	mario = std::make_shared<Mario>();
	drawBlocks();
	drawLadders();
	drawCoins();
	drawMario();
	drawScore();
}

void Game::drawBlocks() {
	_TextureBlock.loadFromFile(BlockTexturePath);
	_sizeBlock = _TextureBlock.getSize();

	int up_base = 0;

	for (int i = 0; i < BASE_BLOCK_COUNT; i++) {
		_Block[i][BLOCK_COUNT_Y].setTexture(_TextureBlock);

		if (i < (BASE_BLOCK_COUNT / 2)) {
			_Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1));
		}
		else {
			up_base += 1;
			_Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1) - up_base);
		}

		std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
		se->entitySprite = _Block[i][BLOCK_COUNT_Y];
		se->entitySize = _TextureBlock.getSize();
		se->entityPosition = _Block[i][BLOCK_COUNT_Y].getPosition();
		EntityManager::managerEntities.push_back(se);
	}

	for (int i = 0; i < BLOCK_COUNT_X; i++) {
		for (int j = 0; j < BLOCK_COUNT_Y; j++) {
			_Block[i][j].setTexture(_TextureBlock);

			if (j % 2) {
				_Block[i][j].setPosition(130.f + 70.f * (i + 1), -5.f + BLOCK_SPACE * (j + 1) + (i + 1));
			}
			else {
				_Block[i][j].setPosition(190.f + 70.f * (i + 1), 5.f + BLOCK_SPACE * (j + 1) - (i + 1));
			}

			std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
			se->entitySprite = _Block[i][j];
			se->entitySize = _TextureBlock.getSize();
			se->entityPosition = _Block[i][j].getPosition();
			EntityManager::managerEntities.push_back(se);
		}
	}
}

void Game::drawLadders() {
	_LadderTexture.loadFromFile(LadderTexturePath);

	for (int i = 0; i <= SCALE_COUNT; i++) {
		_Ladder[i].setTexture(_LadderTexture);

		if (i % 2) {
			_Ladder[i].setPosition(830.f + 70.f, -30.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);
		}
		else {
			_Ladder[i].setPosition(230.f + 70.f, -30.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);
		}

		std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::ladder);
		se->entitySprite = _Ladder[i];
		se->entitySize = _LadderTexture.getSize();
		se->entityPosition = _Ladder[i].getPosition();
		EntityManager::managerEntities.push_back(se);
	}
}

void Game::drawMario() {
	mSpriteSheet.loadFromFile(MarioSpriteSheetPath);

	const int FRAME_WIDTH = 16;
	const int FRAME_HEIGHT = 16;
	const float SCALE_WIDTH = MARIO_WIDTH / FRAME_WIDTH;
	const float SCALE_HEIGHT = MARIO_HEIGHT / FRAME_HEIGHT;

	mario->entitySize = sf::Vector2u(MARIO_WIDTH, MARIO_HEIGHT);
	mario->entityPosition = sf::Vector2f(100.f + 70.f, BLOCK_SPACE * 7 - MARIO_HEIGHT);

	auto standingRightSprite = sf::IntRect(162, 0, FRAME_WIDTH, FRAME_HEIGHT);
	auto standingLeftSprite = sf::IntRect(122, 0, FRAME_WIDTH, FRAME_HEIGHT);
	mario->standingRightRect = sf::Sprite(mSpriteSheet, standingRightSprite);
	mario->standingLeftRect = sf::Sprite(mSpriteSheet, standingLeftSprite);
	mario->entitySprite = mario->standingRightRect;

	mario->standingLeftRect.scale(SCALE_WIDTH, SCALE_HEIGHT);
	mario->standingRightRect.scale(SCALE_WIDTH, SCALE_HEIGHT);

	mario->walkingAnimationRight.setSpriteSheet(mSpriteSheet);
	mario->walkingAnimationRight.addFrame(standingRightSprite);
	mario->walkingAnimationRight.addFrame(sf::IntRect(200, 0, FRAME_WIDTH, FRAME_HEIGHT));
	mario->walkingAnimationRight.addFrame(sf::IntRect(240, 0, FRAME_WIDTH, FRAME_HEIGHT));

	mario->walkingAnimationLeft.setSpriteSheet(mSpriteSheet);
	mario->walkingAnimationLeft.addFrame(standingLeftSprite);
	mario->walkingAnimationLeft.addFrame(sf::IntRect(80, 0, FRAME_WIDTH, FRAME_HEIGHT));
	mario->walkingAnimationLeft.addFrame(sf::IntRect(40, 0, FRAME_WIDTH, FRAME_HEIGHT));

	mario->currentAnimation = &mario->walkingAnimationRight;

	auto animatedSprite = AnimationSprite(sf::seconds(0.1f), true, false);
	animatedSprite.scale(SCALE_WIDTH, SCALE_HEIGHT);
	mario->animatedSprite = animatedSprite;

	EntityManager::managerEntities.push_back(mario);
}

void Game::drawScore() {
	scoreFont.loadFromFile(ScoreFontPath);

	scoreAnnouncementText.setString("Score");
	scoreAnnouncementText.setFont(scoreFont);
	scoreAnnouncementText.setPosition(1080.f, 5.f);
	scoreAnnouncementText.setCharacterSize(40);

	scoreText.setString(std::to_string(score));
	scoreText.setFont(scoreFont);
	scoreText.setPosition(1080.f, 50.f);
	scoreText.setCharacterSize(22);
}

void Game::drawCoins() {
	_CoinTexture.loadFromFile(CoinTexturePath);

	for (int i = 0; i < COIN_COUNT; i++) {
		_Coin[i].setTexture(_CoinTexture);

		int blockX = getRandomNumber(0, BLOCK_COUNT_X);
		int blockY = getRandomNumber(0, BLOCK_COUNT_Y);

		sf::Sprite randomBlock = _Block[blockX][blockY];

		_Coin[i].setPosition(
			randomBlock.getPosition().x,
			0.f + BLOCK_SPACE * i + 2 * _sizeBlock.y
		);

		std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::coin);
		se->entitySprite = _Coin[i];
		se->entitySize = _CoinTexture.getSize();
		se->entityPosition = _Coin[i].getPosition();
		EntityManager::managerEntities.push_back(se);
	}
}

void Game::run() {
	sf::Clock fpsClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()) {
		sf::Time elapsedTime = fpsClock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			handleLadders();
			handleFloors();
			handleCoins();
			update(TimePerFrame);
		}

		updateScore();
		updateStatistics(elapsedTime);
		render();
		if (score == COIN_COUNT*COIN_VALUE) {
			
			winFont.loadFromFile("Media/Sansation.ttf");
			winText.setFont(winFont); 
			winText.setString("WIN");
			winText.setCharacterSize(80);
			winText.setFillColor(sf::Color::Red);
			winText.setStyle(sf::Text::Bold | sf::Text::Underlined);
			mWindow.draw(winText);
		}
	}
}

void Game::processEvents() {
	sf::Event event{};

	while (mWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;

		default:;
		}
	}
}

void Game::update(sf::Time elapsedTime) {
	sf::Vector2f movement(0.f, 0.f);
	if (mario->isJumping && mario->lastJump + jumpTime > clock.getElapsedTime()) {
		movement.y -= MARIO_GRAVITY;
	}
	else {
		mario->isJumping = false;
	}

	if (mario->isFalling && !mario->isOnLadder && !mario->isJumping) {
		movement.y += MARIO_GRAVITY;
	}

	if (mario->isMovingUp && mario->isOnLadder) {
		movement.y -= PlayerSpeed;
	}

	if (mario->isMovingDown && mario->isOnLadder) {
		movement.y += PlayerSpeed;
	}

	if (mario->isMovingLeft) {
		movement.x -= PlayerSpeed;
		mario->currentAnimation = &mario->walkingAnimationLeft;
		mario->isFacingLeft = true;
		mario->isFacingRight = false;
	}

	if (mario->isMovingRight) {
		movement.x += PlayerSpeed;
		mario->currentAnimation = &mario->walkingAnimationRight;
		mario->isFacingRight = true;
		mario->isFacingLeft = false;
	}

	mario->entityPosition.x += movement.x * elapsedTime.asSeconds();
	mario->entityPosition.y += movement.y * elapsedTime.asSeconds();

	mario->animatedSprite.play(*mario->currentAnimation);
	mario->animatedSprite.setPosition(mario->entityPosition);
	mario->animatedSprite.update(elapsedTime);

	if (!mario->isMoving) {
		mario->animatedSprite.stop();

		if (mario->isFacingLeft) {
			mario->entitySprite = mario->standingLeftRect;
		}
		else {
			mario->entitySprite = mario->standingRightRect;
		}

		mario->entitySprite.setPosition(mario->entityPosition);
	}
}

void Game::render() {
	mWindow.clear();

	for (const std::shared_ptr<Entity> &entity : EntityManager::managerEntities) {
		if (entity->isAnimated && entity->isMoving) {
			mWindow.draw(entity->animatedSprite);
		}
		else {
			mWindow.draw(entity->entitySprite);
		}
	}

	mWindow.draw(scoreAnnouncementText);
	mWindow.draw(scoreText);
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateScore() {
	scoreText.setString(std::to_string(score));
	
}

void Game::updateStatistics(sf::Time elapsedTime) {
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}


	if (mStatisticsUpdateTime >= sf::seconds(0.050f)) {
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	switch (key) {
	case sf::Keyboard::Up:
		mario->isMovingUp = mario->isMoving = isPressed;
		break;
	case sf::Keyboard::Down:
		mario->isMovingDown = mario->isMoving = isPressed;
		break;
	case sf::Keyboard::Left:
		mario->isMovingLeft = mario->isMoving = isPressed;
		break;
	case sf::Keyboard::Right:
		mario->isMovingRight = mario->isMoving = isPressed;
		break;

	case sf::Keyboard::Space:
		if (!mario->isFalling) {
			mario->lastJump = clock.getElapsedTime();
			mario->isJumping = isPressed;
		}
		break;
	}
}

void Game::handleCoins() {
	auto coins = EntityManager::GetCoins();
	auto playerBounds = sf::Rect<float>(
		mario->entityPosition.x,
		mario->entityPosition.y,
		mario->entitySize.x,
		mario->entitySize.y
		);

	for (auto const& coin : coins) {
		auto coinGlobalBounds = coin.get()->entitySprite.getGlobalBounds();

		if (coinGlobalBounds.intersects(playerBounds)) {
			EntityManager::RemoveCoin(coin);
			score += COIN_VALUE;
			printf("score : %d", score);
		}
	}
}

void Game::handleFloors() {
	auto floors = EntityManager::GetFloors();
	auto playerBounds = sf::Rect<float>(
		mario->entityPosition.x,
		mario->entityPosition.y,
		mario->entitySize.x,
		mario->entitySize.y
		);
	mario->isFalling = true;
	for (auto const& floor : floors) {
		auto floorGloabalBounds = floor.get()->entitySprite.getGlobalBounds();
		if (floorGloabalBounds.intersects(playerBounds)) {
			mario->isFalling = false;
			return;
		}
	}
}

void Game::handleLadders() {
	auto ladders = EntityManager::GetLadders();
	auto playerBounds = sf::Rect<float>(
		mario->entityPosition.x,
		mario->entityPosition.y,
		mario->entitySize.x,
		mario->entitySize.y
		);
	mario->isOnLadder = false;
	for (auto const& ladder : ladders) {
		auto ladderGloabalBounds = ladder.get()->entitySprite.getGlobalBounds();
		if (ladderGloabalBounds.intersects(playerBounds)) {
			mario->isOnLadder = true;
			return;
		}
	}
}

