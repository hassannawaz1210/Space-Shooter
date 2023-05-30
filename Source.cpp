
#include "Header.h"
#define PLAYER_SPEED 5


// -------------------
//RANDOM(LOWER LIMIT, UPPER LIMIT
//---------------
    sf::Vector2f randCoordsGenerator()
    {
        int x, y;
        do
        {
            x = rand() % (1100 - (-99)) + (-100); // between 1300 to -100
            y = rand() % (800 - (-99)) + (-100); // between 1000 to -100
        } while ((x >= 0 && x <= 1000) && (y >= 0 && y <= 700));
        return sf::Vector2f(x, y);
    }


    void teleportation(sf::Event event, Player &player, Teleport &teleport)
    {
        float x, y;
        x = player.ship.getPosition().x;
        y = player.ship.getPosition().y;
        if(teleport.count > 0)
        {
            if (event.key.code == sf::Keyboard::Key::Space && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player.ship.setPosition(x, y - 50);
                teleport.count--;
            }
            else if (event.key.code == sf::Keyboard::Key::Space && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player.ship.setPosition(x, y + 50);
                teleport.count--;
            }
            else if (event.key.code == sf::Keyboard::Key::Space && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                player.ship.setPosition(x - 50, y);
                teleport.count--;
            }
            else if (event.key.code == sf::Keyboard::Key::Space && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                player.ship.setPosition(x + 50, y);
                teleport.count--;
            }
        }
    }

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(1000, 700) , "Space Shooter");
    window.setFramerateLimit(60);   
    //window.setMouseCursorVisible(0);
    srand(time(0));

    //Player Variable
    Player player;

    //player score
    Score playerScore;

    //levels
    Level level;

    //boss 
    Boss boss;
    sf::Texture bossBulletTex;
	if (!bossBulletTex.loadFromFile("./sprites/boss_bullet.png")) std::cout << "Images couldnt be loaded.\n";
    sf::Clock bossBulletClock;
    std::vector<Bullet> bossBullets;


    //background music
    sf::Music bgMusic;
    bgMusic.openFromFile("./wav/bg.wav");
    bgMusic.setLoop(1);
    bgMusic.play();
    bgMusic.setVolume(40);

    //explosion sound
    sf::SoundBuffer explosion;
    explosion.loadFromFile("./wav/explosion.wav");
    sf::Sound explode(explosion);

    //background picture
    sf::Texture bg_tex;
    bg_tex.loadFromFile("./sprites/stars.png");
    sf::CircleShape bg(600.0);
    bg.setFillColor(sf::Color::White);
    bg.setTexture(&bg_tex);
    //center the bg
    bg.setOrigin(600, 600);
    bg.setPosition(500, 350);
    //------------

    //Variables
    sf::Vector2f mouseCoords;
    sf::Vector2f unitVector;
    sf::Vector2f enemyDir;
    sf::Vector2f enemyPos;
    sf::Clock clock; // starts the clock

    //explosion animation
    float deltaTime = 0;
    sf::Texture explosion_tex;
    explosion_tex.loadFromFile("./sprites/explosion.png");
    sf::RectangleShape explosion_sprite(sf::Vector2f(200, 250));
    explosion_sprite.setOrigin(100, 125);
    explosion_sprite.setTexture(&explosion_tex);
    sf::Clock explosion_clock;
    Explosion explosion_anim(&explosion_tex, 7, 0.1f);

    //power move
    PowerMove powerMove;

    //teleport
    Teleport teleport;
    sf::Clock teleportClock;

    //Enemies
    Enemy enemy;
    std::vector<Enemy> enemies;
    //enemy texures variables
    sf::Texture enemy_texture;
    if (!enemy_texture.loadFromFile("./sprites/enemy_1.png")) std::cout << "Images couldnt be loaded.\n";
 

    //bullets
    Bullet bullet;
    std::vector<Bullet> bullets;
    sf::Texture bullet_tex;
    if (!bullet_tex.loadFromFile("./sprites/bullet.png")) std::cout << "Images couldnt be loaded.\n";

    
    //----
    sf::Event event;
    while (window.isOpen())
    {
        //updating delta time
        deltaTime  = explosion_clock.restart().asSeconds();


        //rotating bankground
        bg.rotate(0.05);
        //------ taking input
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::EventType::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::EventType::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Key::Escape) window.close();

                teleportation(event, player, teleport);

                break;
            }
            case sf::Event::EventType::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left && !player.ship.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) //if mouse is not on player
                {
                    //rotate the bullet in the direction of the unit vector
                    bullet.shell.setRotation((atan2(unitVector.y, unitVector.x) * 180 / 3.14) + 90);
                    bullet.shell.setPosition(sf::Vector2f(player.ship.getPosition().x + (unitVector.x * 80), player.ship.getPosition().y + (unitVector.y * 80)));
                    bullet.shell.setTexture(&bullet_tex);
                    bullet.currVelocity = unitVector * bullet.maxSpeed;
                    bullets.push_back(bullet);
                }
                // power move
                else if (event.mouseButton.button == sf::Mouse::Right && powerMove.count >= 3)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        bullet.shell.setTexture(&bullet_tex);
                        bullet.shell.setRotation(atan2(powerMove.direction[i].y, powerMove.direction[i].x) * 180 / 3.14 + 90);
                        bullet.shell.setPosition(sf::Vector2f(player.ship.getPosition().x + (powerMove.direction[i].x * 80), player.ship.getPosition().y + (powerMove.direction[i].y * 80)));
                        bullet.currVelocity = powerMove.direction[i] * bullet.maxSpeed;
                        bullets.push_back(bullet);
                    }
                    powerMove.count = 0;
				}
                break;
            }
            case sf::Event::EventType::MouseMoved:
            {
                if (player.ship.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) break;

                mouseCoords = sf::Vector2f(sf::Mouse::getPosition(window));

                sf::Vector2f difVector = mouseCoords - player.ship.getPosition();
                unitVector = difVector / float(sqrt(pow(difVector.x, 2) + pow(difVector.y, 2)));

                float angle = atan2(unitVector.y, unitVector.x) * 180 / 3.14;
                player.ship.setRotation(angle + 90);
                break;
            }
            
            default:
                break;
            }
        }

        //------- updating frame

        //player movement wasd
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            //if player within bounds
            if(player.ship.getPosition().y > 0)
			player.ship.move(0, -PLAYER_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            if (player.ship.getPosition().y < 700)
             player.ship.move(0, PLAYER_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            if (player.ship.getPosition().x > 0)
			 player.ship.move(-PLAYER_SPEED, 0);
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            if (player.ship.getPosition().x < 1000)
             player.ship.move(PLAYER_SPEED, 0);
        }

        //spawning a boss
        if (boss.bossLevel(level.currentLevel))
        {
            boss.updateBoss(player, unitVector, bullets);
        }

        //boss shooting
        if(bossBulletClock.getElapsedTime().asSeconds() >= 1)
        if (boss.bossLevel(level.currentLevel))
        {
            //find the unit vector between player and boss
            sf::Vector2f difVector = player.ship.getPosition() - boss.body.getPosition();
            sf::Vector2f directionVector = difVector / float(sqrt(pow(difVector.x, 2) + pow(difVector.y, 2)));
            //rotate the bullet in the direction of the unit vector
            bullet.shell.setRotation((atan2(directionVector.y, directionVector.x) * 180 / 3.14) + 90);
            bullet.shell.setPosition(sf::Vector2f(boss.body.getPosition().x + (directionVector.x * 80), boss.body.getPosition().y + (directionVector.y * 80)));
            bullet.shell.setTexture(&bossBulletTex);
            bullet.currVelocity = directionVector * bullet.maxSpeed;
            bossBullets.push_back(bullet);

            bossBulletClock.restart();
        }

        
        //finding direction for enemy and setting it in motion
        if(level.currentLevel == 1)
        if (int(clock.getElapsedTime().asMilliseconds()) >=  600)
        {
            enemy.updateEnemy();
            enemy.body.setTexture(&enemy_texture);
            //find the direction
            enemyPos = enemy.body.getPosition();
            enemyDir = enemyPos - player.ship.getPosition();
            enemyDir = -enemyDir / float(sqrt(pow(enemyDir.x, 2) + pow(enemyDir.y, 2)));

            enemy.currVelocity = enemyDir * enemy.maxSpeed;
            enemies.push_back(enemy);

            clock.restart();
        }

        //enemies movement
        for (int i = 0; i < enemies.size(); i++)
        {
            enemies[i].body.rotate(1);
            enemies[i].body.move(enemies[i].currVelocity);
        }

        //bullets movement
        for (int i = 0; i < bullets.size(); i++)
        {
            bullets[i].shell.move(bullets[i].currVelocity);
        }

        //boss bullets movement
        for (int i = 0; i < bossBullets.size(); i++)
		{
			bossBullets[i].shell.move(bossBullets[i].currVelocity);
		}

        //deleting the out of bounds bullets
        for (int i = 0; i < bullets.size(); i++)
        {
            if(bullets[i].shell.getPosition().x < 0 || bullets[i].shell.getPosition().x > window.getSize().x ||
                bullets[i].shell.getPosition().y < 0 || bullets[i].shell.getPosition().y > window.getSize().y)
             bullets.erase(bullets.begin() + i);
        }

        //deleting the enemies upon colliding with the ship
        for (int i = 0; i < enemies.size(); i++)
        {

            sf::Vector2f difVector = enemies[i].body.getPosition() - player.ship.getPosition();
            float distance = sqrt(pow(difVector.x, 2) + pow(difVector.y, 2));
            float sumOfRadii = enemies[i].body.getRadius() + player.ship.getSize().x / 2;
            if (distance < sumOfRadii)
            {
                enemies.erase(enemies.begin() + i);
                explode.play();
            }
        }

        //deleting the out of bounds bossBullets
        for (int i = 0; i < bossBullets.size(); i++)
		{
			if (bossBullets[i].shell.getPosition().x < 0 || bossBullets[i].shell.getPosition().x > window.getSize().x ||
				bossBullets[i].shell.getPosition().y < 0 || bossBullets[i].shell.getPosition().y > window.getSize().y)
				bossBullets.erase(bossBullets.begin() + i);
		}

        //deleting the bossBullets upon colliding with the ship
        for (int i = 0; i < bossBullets.size(); i++)
		{

			sf::Vector2f difVector = bossBullets[i].shell.getPosition() - player.ship.getPosition();
			float distance = sqrt(pow(difVector.x, 2) + pow(difVector.y, 2));
			float sumOfRadii = bossBullets[i].shell.getRadius() + player.ship.getSize().x / 2;
			if (distance < sumOfRadii)
			{
				bossBullets.erase(bossBullets.begin() + i);
				explode.play();
			}
		}

        ////collision between player bullets and boss bullets
        //for (int i = 0; i < bullets.size(); i++)
		      //  {
			     //   for (int j = 0; j < bossBullets.size(); j++)
			     //   {
				    //    if (i >= bullets.size() || j >= bossBullets.size()) continue;

				    //    sf::Vector2f difVector = bullets[i].shell.getPosition() - bossBullets[j].shell.getPosition();
				    //    float distance = sqrt(pow(difVector.x, 2) + pow(difVector.y, 2));
				    //    float sumOfRadii = bullets[i].shell.getRadius() + bossBullets[j].shell.getRadius();
				    //    if (distance < sumOfRadii)
				    //    {
        //                    //explosion stuff
        //                    explode.play();
        //                    explosion_sprite.setPosition(enemies[i].body.getPosition());
        //                    explosion_anim.flag = true;
        //                    explosion_anim.currImage = 0;

        //                    //deleting the bullets
					   //     bullets.erase(bullets.begin() + i);
					   //     bossBullets.erase(bossBullets.begin() + j);
					   //     
				    //    }
			     //   }
		      //  }


        //collision between bullets and enemies
        for (int i = 0; i < enemies.size(); i++)
            for (int j = 0; j < bullets.size(); j++)
            {
                if (i >= enemies.size() || j >= bullets.size()) continue;

                sf::Vector2f difVector = enemies[i].body.getPosition() - bullets[j].shell.getPosition();
                float distance = sqrt(pow(difVector.x, 2) + pow(difVector.y, 2));
                float sumOfRadii = enemies[i].body.getRadius() + bullets[j].shell.getRadius();
                if (distance < sumOfRadii)
                {
                    //explosion stuff
                    explode.play();
                    explosion_sprite.setPosition(enemies[i].body.getPosition());
                    explosion_anim.flag = true;
                    explosion_anim.currImage = 0;

                    //adding to player score
                    player.score += 10;

                    //power move count
                    powerMove.count++;

                    //deleting the enemies and bullets
                    enemies.erase(enemies.begin() + i);
                    bullets.erase(bullets.begin() + j);
                }
            }

        //collision between bullets and boss
        for (int i = 0; i < bullets.size(); i++)
        {
            if (boss.body.getGlobalBounds().contains(bullets[i].shell.getPosition()))
            {
                bullets.erase(bullets.begin() + i);

                //explosion stuff
                explode.play();
                explosion_sprite.setPosition(boss.body.getPosition());
                explosion_anim.flag = true;
                explosion_anim.currImage = 0;

                //adding to player score
                player.score += 10;

                //power move count
                powerMove.count++;
            }
		}

        //updating teleport
        if (teleportClock.getElapsedTime().asSeconds() > 1)
        {
            teleport.count++;
            teleportClock.restart();
        }
		teleport.updateTeleportRect();

        //updating the explosions
        explosion_anim.Update(deltaTime);
        explosion_sprite.setTextureRect(explosion_anim.Image);

        //update player score
        playerScore.updateScore(player.score);

        //update level
        level.updateLevel();

        //update power rect
        powerMove.updatePowerRect();

        //--------------------------------------------------------------------------

        //------- clearing previous screen
        window.clear(sf::Color::Black);

        //------- re-drawing/rendering stuff

        //drawing background
        window.draw(bg);

        //player score text
        window.draw(playerScore.scoreText);

        //drawing level text
        window.draw(level.levelText);

        //drawing teleport rect and text
        window.draw(teleport.lowerRect);
		window.draw(teleport.upperRect);
		window.draw(teleport.teleportText);


        //drawing player ship
        window.draw(player.ship);

        //drawing power rect
        window.draw(powerMove.lowerRect);
        window.draw(powerMove.upperRect);
        window.draw(powerMove.powerMoveText);

        //drawing boss
        if (boss.bossLevel(level.currentLevel))
        {
			window.draw(boss.body);
		}

        //drawing boss bullets
        for (int i = 0; i < bossBullets.size(); i++)
		{
			window.draw(bossBullets[i].shell);
		}
          
        //drawing bullets
        for (int i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i].shell);
        }

        //drawing explosions
        if(explosion_anim.flag) window.draw(explosion_sprite);

        //drawing enemies
        for (int i = 0; i < enemies.size(); i++)
        {
            window.draw(enemies[i].body);
        }

        //------ displaying the canvas
        window.display();
    }
    
    return 0;
}