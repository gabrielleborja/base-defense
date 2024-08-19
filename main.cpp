#include <SFML/Graphics.hpp>
#include "projectile.h"
#include "base.h"
#include "hero.h"
#include "enemy.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
    const unsigned int windowWidth = 1200;
    const unsigned int windowHeight = 800;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Base Defense");

    const float baseWidth = 600.0f;
    const float baseHeight = 400.0f;

    sf::Vector2f basePosition(
        ((windowWidth - baseWidth)+ 50.0 )/ 2.0f,
        ((windowHeight - baseHeight) -10.0)/ 2.0f
    );

    Base base(basePosition, 100.0f, 5.0f, "base.png");
    base.getSprite().setScale(sf::Vector2f(baseWidth, baseHeight));
    base.getSprite().setPosition(basePosition);

    sf::RectangleShape healthBarBackground;
    healthBarBackground.setSize(sf::Vector2f(200.0f, 20.0f));
    healthBarBackground.setFillColor(sf::Color::Red);
    healthBarBackground.setPosition(windowWidth - 210.0f, 10.0f);

    sf::RectangleShape healthBar;
    healthBar.setSize(sf::Vector2f(200.0f, 20.0f));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(windowWidth - 210.0f, 10.0f);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
        return -1;
    }

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(400, 350);

    Hero hero(sf::Vector2f(windowWidth / 2.0f - 25.0f, windowHeight / 2.0f - 25.0f), 200.0f, "heroi.png");

    std::vector<Projectile> projectiles;
    std::vector<Enemy> enemies;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::Clock clock;
    sf::Clock enemySpawnClock; // Relógio para controlar o tempo de spawn dos inimigos
    sf::Clock gameOverClock;   // Relógio para controlar o tempo após o game over

    bool isGameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!isGameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                sf::Vector2f heroPos = hero.getShape().getPosition();
                sf::Vector2f direction = mousePosF - heroPos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) {
                    direction /= length; // Normaliza a direção
                }

                float projectileSpeed = 300.0f;
                float projectileRange = 700.0f;
                projectiles.emplace_back(heroPos + sf::Vector2f(25.0f, 25.0f), direction, projectileSpeed, projectileRange, true); // true indica que o projétil é do herói
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if (!isGameOver) {
            // Adicionar um novo inimigo a cada três segundos
            if (enemySpawnClock.getElapsedTime().asSeconds() >= 4.0f) {
                enemySpawnClock.restart();

                // Definir a posição inicial do inimigo em uma das bordas
                float xPos, yPos;
                int edge = std::rand() % 4;

                switch (edge) {
                    case 0: // Topo
                        xPos = static_cast<float>(std::rand() % windowWidth);
                        yPos = 0.0f;
                        break;
                    case 1: // Base
                        xPos = static_cast<float>(std::rand() % windowWidth);
                        yPos = static_cast<float>(windowHeight);
                        break;
                    case 2: // Esquerda
                        xPos = 0.0f;
                        yPos = static_cast<float>(std::rand() % windowHeight);
                        break;
                    case 3: // Direita
                        xPos = static_cast<float>(windowWidth);
                        yPos = static_cast<float>(std::rand() % windowHeight);
                        break;
                }

                enemies.emplace_back(sf::Vector2f(xPos, yPos), 50.0f, "inimigos.png"); // Velocidade reduzida
            }

            for (auto it = projectiles.begin(); it != projectiles.end(); /* vazio */) {
                it->update(deltaTime);

                bool removed = false;
                if (it->hasReachedMaxRange()) {
                    it = projectiles.erase(it);
                    removed = true;
                } else if (it->isHero()) {
                    // Verifica colisão com inimigos
                    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); /* vazio */) {
                        if (it->checkCollision(enemyIt->getSprite())) {
                            enemyIt->takeDamage();
                            it = projectiles.erase(it);
                            removed = true;
                            break; // Projétil destruído, sair do loop de inimigos
                        } else {
                            ++enemyIt;
                        }
                    }
                } else {
                    // Verifica colisão com a base
                    if (it->checkCollision(base.getSprite())) {
                        base.takeDamage(3.0f);  // Causa 5 de dano à base
                        it = projectiles.erase(it); // Remove o projétil após o dano
                        removed = true;
                    }
                }

                if (!removed) {
                    ++it;
                }
            }

            // Atualizar inimigos
            for (auto it = enemies.begin(); it != enemies.end(); /* vazio */) {
                if (it->isDead()) {
                    it = enemies.erase(it);
                } else {
                    it->update(deltaTime, base.getSprite(), projectiles); // Passa a lista de projéteis
                    ++it;
                }
            }

            //base.regenerate(deltaTime);

            float healthPercentage = base.getCurrentHealth() / 100.0f;
            healthBar.setSize(sf::Vector2f(200.0f * healthPercentage, 20.0f));

            if (base.getCurrentHealth() <= 0.0f) {
                isGameOver = true;
                gameOverClock.restart(); // Inicializa o relógio ao detectar o game over
            }
        }

        window.clear();

        if (!isGameOver) {
            base.draw(window);
            window.draw(healthBarBackground);
            window.draw(healthBar);

            hero.handleInput();
            hero.update(deltaTime);
            hero.draw(window);

            for (auto& projectile : projectiles) {
                projectile.draw(window);
            }

            for (auto& enemy : enemies) {
                enemy.draw(window);
            }
        } else {
            window.draw(gameOverText);

            // Fechar a janela após 3 segundos de exibição da mensagem de Game Over
            if (gameOverClock.getElapsedTime().asSeconds() >= 3.0f) {
                window.close();
            }
        }

        window.display();
    }

    return 0;
}
