#include <SFML/Graphics.hpp>
#include "projectile.h"
#include "base.h"
#include "enemy.h"
#include "hero.h"
#include <vector>
#include <cstdlib> // Para std::rand()
#include <ctime>   // Para std::time()

int main()
{
    const unsigned int windowWidth = 1200;
    const unsigned int windowHeight = 800;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Base Defense");
    window.setVerticalSyncEnabled(false);

    // Carregar a imagem de fundo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("bg.png"))
    {
        return -1; // Se a imagem não for carregada, saia com erro
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    Base base(sf::Vector2f((windowWidth - 600.0f) / 2.0f, (windowHeight - 400.0f) / 2.0f), 100.0f, 5.0f);
    Hero hero(sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.0f), 100.0f, 10);

    sf::RectangleShape healthBarBackground;
    healthBarBackground.setSize(sf::Vector2f(200.0f, 20.0f));
    healthBarBackground.setFillColor(sf::Color::Red);
    healthBarBackground.setPosition(windowWidth - 210.0f, 10.0f);

    sf::RectangleShape healthBar;
    healthBar.setSize(sf::Vector2f(200.0f, 20.0f));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(windowWidth - 210.0f, 10.0f);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return -1;
    }

    sf::Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(24);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(10.0f, 10.0f);

    std::vector<Projectile> projectiles;
    std::vector<Enemy> enemies;

    sf::Clock clock;
    sf::Time enemySpawnTimer;
    const sf::Time spawnInterval = sf::seconds(2.0f);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                hero.shoot(projectiles, mousePosF);
            }
        }

        if (base.isDestroyed())
        {
            window.close();
            continue;
        }

        float deltaTime = clock.restart().asSeconds();
        hero.move(deltaTime);

        enemySpawnTimer += sf::seconds(deltaTime);
        if (enemySpawnTimer >= spawnInterval)
        {
            enemySpawnTimer = sf::Time::Zero;
            sf::Vector2f spawnPosition;
            sf::Vector2f direction;
            float speed = 100.0f;
            float health = 50.0f;
            float projectileCooldown = 2.0f;

            int side = std::rand() % 4;
            switch (side)
            {
            case 0: // Top
                spawnPosition = sf::Vector2f(std::rand() % windowWidth, 0.0f);
                direction = sf::Vector2f(0.0f, 1.0f);
                break;
            case 1: // Right
                spawnPosition = sf::Vector2f(static_cast<float>(windowWidth), std::rand() % windowHeight);
                direction = sf::Vector2f(-1.0f, 0.0f);
                break;
            case 2: // Bottom
                spawnPosition = sf::Vector2f(std::rand() % windowWidth, static_cast<float>(windowHeight));
                direction = sf::Vector2f(0.0f, -1.0f);
                break;
            case 3: // Left
                spawnPosition = sf::Vector2f(0.0f, std::rand() % windowHeight);
                direction = sf::Vector2f(1.0f, 0.0f);
                break;
            }
            enemies.emplace_back(spawnPosition, direction, speed, health, projectileCooldown);
        }

        for (auto it = projectiles.begin(); it != projectiles.end(); /* vazio */)
        {
            if (it->hasReachedMaxRange())
            {
                it = projectiles.erase(it);
            }
            else
            {
                it->update(deltaTime);

                bool erased = false;
                for (auto &enemy : enemies)
                {
                    if (it->checkCollision(enemy.getShape()))
                    {
                        enemy.takeDamage(10.0f);
                        if (enemy.isDestroyed())
                        {
                            hero.addAmmo(5);
                        }
                        it = projectiles.erase(it);
                        erased = true;
                        break;
                    }
                }
                if (!erased)
                {
                    ++it;
                }
            }
        }

        for (auto it = enemies.begin(); it != enemies.end(); /* vazio */)
        {
            it->update(deltaTime, projectiles, base.getShape().getPosition());
            if (it->getShape().getGlobalBounds().intersects(base.getShape().getGlobalBounds()))
            {
                base.takeDamage(10.0f);
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }

        base.regenerate(deltaTime);

        float healthPercentage = base.getCurrentHealth() / 100;
        healthBar.setSize(sf::Vector2f(200.0f * healthPercentage, 20.0f));

        ammoText.setString("Ammo: " + std::to_string(hero.getAmmo()));

        window.clear();
        window.draw(backgroundSprite); // Desenha o fundo
        base.draw(window);
        hero.draw(window);
        window.draw(healthBarBackground);
        window.draw(healthBar);
        window.draw(ammoText);

        for (auto &projectile : projectiles)
        {
            projectile.draw(window);
        }

        for (auto &enemy : enemies)
        {
            enemy.draw(window);
        }

        window.display();
    }

    return 0;
}
