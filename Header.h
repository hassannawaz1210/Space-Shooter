#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/audio.hpp>
#include <iostream>
#include <ctime>
#include <vector>

sf::Vector2f randCoordsGenerator();

class Player
{
public:
	sf::RectangleShape ship;
	sf::Texture ship_tex;
	int score;

	Player(): ship(sf::Vector2f(120, 120)) // default values
	{
		ship.setFillColor(sf::Color::White);
		ship.setPosition(sf::Vector2f(500, 350));
		ship.setOrigin(sf::Vector2f(60, 60));

		//loading texture for space ship
		if (!ship_tex.loadFromFile("ship.png")) std::cout << "Images wasnt loaded successfully\n";
		//applying texture
		ship.setTexture(&ship_tex);
	}

};

class Bullet
{
public:
	sf::CircleShape shell;
	sf::Vector2f currVelocity;
	float maxSpeed;

	Bullet(float radius = 30.0f): currVelocity(0.f, 0.f), maxSpeed(5.f)
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
		if(maxSpeed<5.0)
		maxSpeed += 0.1;

	}

};