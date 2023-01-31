
#include "Header.h"



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


int main()
{
    
    sf::RenderWindow window(sf::VideoMode(1000, 700) , "Space Shooter");
    window.setFramerateLimit(60);   
    //window.setMouseCursorVisible(0);
    srand(time(0));

    //Player Variable
    Player player;

    //background music
    sf::Music bgMusic;
    bgMusic.openFromFile("bg.wav");
    bgMusic.setLoop(1);
    bgMusic.play();
    bgMusic.setVolume(40);

    //explosion sound
    sf::SoundBuffer explosion;
    explosion.loadFromFile("explosion.wav");
    sf::Sound explode(explosion);


    //background picture
    sf::Texture bg_tex;
    bg_tex.loadFromFile("stars.png");
    sf::RectangleShape bg(sf::Vector2f(1300, 1000));
    bg.setFillColor(sf::Color::White);
    bg.setTexture(&bg_tex);
    bg.setPosition(650, 500);
    bg.setOrigin(650, 500);
    //------------


    //Variables
    sf::Vector2f mouseCoords;
    sf::Vector2f unitVector;
    sf::Vector2f enemyDir;
    sf::Vector2f enemyPos;
    sf::Clock clock; // starts the clock


    //Enemies
    Enemy enemy;
    std::vector<Enemy> enemies;
    //enemy texures variables
    sf::Texture enemy_texture;
    if (!enemy_texture.loadFromFile("enemy_1.png")) std::cout << "Images couldnt be loaded.\n";
 

    //bullets
    Bullet bullet;
    std::vector<Bullet> bullets;
    sf::Texture bullet_tex;
    if (!bullet_tex.loadFromFile("bullet.png")) std::cout << "Images couldnt be loaded.\n";

    
    //----
    sf::Event event;
    while (window.isOpen())
    {
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
                break;
            }
            case sf::Event::EventType::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    bullet.shell.setPosition(sf::Vector2f((unitVector.x * 80) + 500, (unitVector.y * 80) + 350));
                    bullet.shell.setTexture(&bullet_tex);
                    bullet.currVelocity = unitVector * bullet.maxSpeed;
                    bullets.push_back(bullet);
                }
            }
            case sf::Event::EventType::MouseMoved:
            {
                if (player.ship.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) break;

                mouseCoords = sf::Vector2f(sf::Mouse::getPosition(window));

                sf::Vector2f difVector = mouseCoords - player.ship.getPosition();
                unitVector = difVector / float(sqrt(pow(difVector.x, 2) + pow(difVector.y, 2)));

                float angle = atan2(unitVector.y, unitVector.x) * 180 / 3.14;
                player.ship.setRotation(angle);
                break;
            }
            
            default:
                break;
            }
        }

        //------- updating frame

        //finding direction for enemy and setting it in motion
        if (int(clock.getElapsedTime().asSeconds()) == 1)
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
            bullets[i].shell.rotate(5);
            bullets[i].shell.move(bullets[i].currVelocity);
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
            if (distance < sumOfRadii)  enemies.erase(enemies.begin() + i);
        }

        //collision between bullets and enemies
        for (int i = 0; i < enemies.size(); i++)
            for (int j = 0; j < bullets.size(); j++)
            {
                sf::Vector2f difVector = enemies[i].body.getPosition() - bullets[j].shell.getPosition();
                float distance = sqrt(pow(difVector.x, 2) + pow(difVector.y, 2));
                float sumOfRadii = enemies[i].body.getRadius() + bullets[j].shell.getRadius();
                if (distance < sumOfRadii)
                {
                    explode.play();
                    enemies.erase(enemies.begin() + i);
                    bullets.erase(bullets.begin() + j);
                }
            }



        //------- clearing previous screen
        window.clear(sf::Color::Black);

        //------- re-drawing/rendering stuff
        window.draw(bg);
        window.draw(player.ship);

        //drawing bullets
        for (int i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i].shell);
        }

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