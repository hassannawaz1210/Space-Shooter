#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/audio.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700


//----------- degrees into radians
double deg_to_rad(double degree)
{
	double pi = 3.14159265359;
	return (degree * (pi / 180));
}
//--------------------

sf::Vector2f randCoordsGenerator();

class Player
{
public:
	sf::RectangleShape ship;
	sf::Texture ship_tex;
	int score;
	int x_coord = 0, y_coord = 0;

	Player(): ship(sf::Vector2f(80, 80)), x_coord(ship.getPosition().x), y_coord(ship.getPosition().y)// default values
	{
		ship.setFillColor(sf::Color::White);
		ship.setPosition(sf::Vector2f(500, 350));
		ship.setOrigin(sf::Vector2f(40, 40));
		score = 0;

		//loading texture for space ship
		if (!ship_tex.loadFromFile("./sprites/ship.png")) std::cout << "Error loading ship texture.\n";
		//applying texture
		ship.setTexture(&ship_tex);
	}

};

class Bullet
{
public:
	sf::CircleShape shell;
	sf::Vector2f currVelocity;
	bool hasBeenConsideredForDodging = false;
	float maxSpeed;

	Bullet(float radius = 20.0f): currVelocity(0.f, 0.f), maxSpeed(10.f)
	{
		shell.setOrigin(radius, radius);
		shell.setRadius(radius);
		shell.setFillColor(sf::Color::White);
	}
};

class Enemy
{
public:
	sf::CircleShape body;
	sf::Vector2f currVelocity;
	float maxSpeed;

	Enemy(): currVelocity(0.f, 0.f), maxSpeed(1.f)
	{
		body.setFillColor(sf::Color::White);
	}

	void updateEnemy()
	{
		//things that need changing before spawning a new enemy
		float enemySize = (rand() % 61) + 20; //between 60 and 20
		body.setPosition(randCoordsGenerator());
		body.setRadius(enemySize);
		body.setOrigin(body.getRadius(), body.getRadius());
		if(maxSpeed<10.0)
		maxSpeed += 0.1;
	}

};

class Explosion
{
	int imageCount = 0;
	float switchTime = 0;
public:
	int currImage = 0;
	bool flag;
	sf::IntRect Image;
	float totalTime;
	int coordsArray[7];

	Explosion(sf::Texture* texture, int imageCount, float switchTime)
	{
		flag = false;
		this->imageCount = imageCount;
		this->switchTime = switchTime;
		totalTime = 0;
		currImage = 0;
		Image.width = (*texture).getSize().x / float(imageCount);
		Image.height = (*texture).getSize().y;
		
		//hard coded because no appropriate sprite image was found
		coordsArray[0] = 0;
		coordsArray[1] = 155;
		coordsArray[2] = 288;
		coordsArray[3] = 435;
		coordsArray[4] = 595;
		coordsArray[5] = 775;
		coordsArray[6] = 930;
	}
	void Update(const float deltaTime)
	{
		totalTime += deltaTime;
		if (totalTime >= switchTime)
		{
			currImage++;
			totalTime -= switchTime;
			if (currImage >= imageCount)
			{
				currImage = 0;
				flag = false;
			}
		}
		Image.left = coordsArray[currImage];
		Image.top =  0;
	}

};


class PowerMove
{
public:
	int count = 0;
	int maxCount = 3;
	sf::Text powerMoveText;
	sf::Font font;
	sf::RectangleShape lowerRect;
	sf::RectangleShape upperRect;
	std::vector<sf::Vector2f> direction;
	PowerMove()
	{
		if(!font.loadFromFile("./fonts/score_font.ttf")) std::cout << "Error loading powermove font.\n";
		//
		powerMoveText.setFont(font);
		powerMoveText.setCharacterSize(20);
		powerMoveText.setFillColor(sf::Color::White);
		powerMoveText.setPosition(sf::Vector2f(20, 630));
		powerMoveText.setString("Power");
		//
		lowerRect.setFillColor(sf::Color(255, 0, 0, 100));
		lowerRect.setSize(sf::Vector2f(100, 30));
		lowerRect.setPosition(sf::Vector2f(20, 660));
		//
		upperRect.setFillColor(sf::Color::Red);
		upperRect.setPosition(sf::Vector2f(20, 660));

		direction.push_back(sf::Vector2f(cos(deg_to_rad(0)),sin(deg_to_rad(0))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(45)),sin(deg_to_rad(45))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(90)),sin(deg_to_rad(90))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(135)),sin(deg_to_rad(135))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(180)),sin(deg_to_rad(180))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(225)),sin(deg_to_rad(225))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(270)),sin(deg_to_rad(270))));
		direction.push_back(sf::Vector2f(cos(deg_to_rad(315)), sin(deg_to_rad(315))));
	}

	void updatePowerRect()
	{
		if(count>maxCount) count = maxCount;
		upperRect.setSize(sf::Vector2f(33.3 * count, 30));
	}
};

class Teleport
{
public:

	int count = 0;
	int maxCount = 3;
	sf::Text teleportText;
	sf::Font font;
	sf::RectangleShape lowerRect;
	sf::RectangleShape upperRect;

	Teleport()
	{
		if(!font.loadFromFile("./fonts/score_font.ttf")) std::cout << "Error loading teleport font.\n";
		teleportText.setFont(font);
		teleportText.setCharacterSize(20);
		teleportText.setFillColor(sf::Color::White);
		teleportText.setPosition(sf::Vector2f(880, 630));
		teleportText.setString("Teleport");
		//set lower rect color to light blue
		lowerRect.setFillColor(sf::Color(0, 255, 255, 100));
		lowerRect.setSize(sf::Vector2f(100, 30));
		lowerRect.setPosition(sf::Vector2f(880, 660));
		//
		upperRect.setFillColor(sf::Color::Blue);
		upperRect.setPosition(sf::Vector2f(880, 660));
	}

		void updateTeleportRect()
	{
		if(count>maxCount) count = maxCount;
		upperRect.setSize(sf::Vector2f(33.3 * count, 30));
	}

};

class Score
{
public:
	sf::Text scoreText;
	sf::Font font;
	Score()
	{
		if (!font.loadFromFile("./fonts/score_font.ttf")) std::cout << "Error loading score font.\n";
		scoreText.setFont(font);
		scoreText.setCharacterSize(30);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setPosition(sf::Vector2f(10, 10));
	}
	void updateScore(int score)
	{
		scoreText.setString("Score: " + std::to_string(score));
	}
};


class Level
{
	public:
	int currentLevel;
	sf::Text levelText;
	sf::Font font;
	sf::Clock clock;

	Level()
	{
		clock.restart();
		currentLevel = 1;
		if (!font.loadFromFile("./fonts/score_font.ttf")) std::cout << "Error loading score font.\n";
		levelText.setFont(font);
		levelText.setCharacterSize(30);
		levelText.setFillColor(sf::Color::White);
		levelText.setPosition(sf::Vector2f(870, 10));
	}

	void updateLevel()
	{
		levelText.setString("Level: " + std::to_string(currentLevel));
		int minutes = 1;
		if (clock.getElapsedTime().asSeconds() >= (minutes * 1) && currentLevel == 1)
		{
			currentLevel++;
			clock.restart();
		}
	}
};

class Boss
{
public:
	sf::RectangleShape body;
	sf::Vector2f currVelocity;
	sf::Texture bossTexture;
	float maxSpeed;

	Boss() : currVelocity(0.f, 0.f), maxSpeed(1.f)
	{
		maxSpeed = 1;
		body.setPosition(sf::Vector2f(500, 100));
		body.setFillColor(sf::Color::White);
		if (!bossTexture.loadFromFile("./sprites/boss.png")) std::cout << "Error loading boss texture.\n";
		body.setTexture(&bossTexture);
		body.setSize(sf::Vector2f(100, 100));
		body.setOrigin(body.getSize().x / 2, body.getSize().y / 2);
	}

	bool bossLevel(int currentLevel)
	{
		if (currentLevel == 2) return true;
		else return false;
	}

	void updateBoss(Player& player, sf::Vector2f playerAim, std::vector<Bullet>& bullets)
	{
		sf::Vector2f direction = player.ship.getPosition() - body.getPosition();
		float magnitude = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		direction.x = direction.x / magnitude;
		direction.y = direction.y / magnitude;
		if (!player.ship.getGlobalBounds().intersects(body.getGlobalBounds()))
		{
			body.setRotation(atan2(direction.y, direction.x) * 180 / 3.14 + 90);
		}

		for (auto& bullet : bullets)
		{
			if (!bullet.hasBeenConsideredForDodging && isBossInAimDirection(player, playerAim))
			{
				// Generate random number to determine whether to move left or right
				bool moveLeft = rand() % 2 == 0;

				if (moveLeft) {
					// Move the boss left
					float newXPos = body.getPosition().x - (200 + 1);
					if (newXPos >= 0) {
						body.setPosition(newXPos, body.getPosition().y);
					}
				}
				else {
					// Move the boss right
					float newXPos = body.getPosition().x + ( 200 + 1);
					if (newXPos <= WINDOW_WIDTH) {
						body.setPosition(newXPos, body.getPosition().y);
					}
				}

				// Generate random number to determine whether to teleport to opposite coordinates
				bool teleport = rand() % 2 == 0;

				if (teleport) {
					// Teleport the boss to opposite coordinates
					float newXPos = WINDOW_WIDTH - body.getPosition().x;
					float newYPos = WINDOW_HEIGHT - body.getPosition().y;
					body.setPosition(newXPos, newYPos);
				}

				// Set the flag to true so the bullet isn't considered again
				bullet.hasBeenConsideredForDodging = true;
			}
		}
	}

		bool isBossInAimDirection(Player& player, sf::Vector2f playerAim)
	{
		sf::Vector2f boss = body.getPosition() - player.ship.getPosition();
		float bossMagnitude = sqrt(pow(boss.x, 2) + pow(boss.y, 2));
		boss.x = boss.x / bossMagnitude;
		boss.y = boss.y / bossMagnitude;
		float dotProduct = boss.x * playerAim.x + boss.y * playerAim.y;
		return dotProduct > 0.9;
	}

	

};