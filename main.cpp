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
        ((windowWidth - baseWidth) + 50.0f) / 2.0f,
        ((windowHeight - baseHeight) - 10.0f) / 2.0f
    );

    Base base(basePosition, 100.0f, 5.0f, "base.png");
    base.getSprite().setScale(sf::Vector2f(baseWidth / base.getSprite().getLocalBounds().width, baseHeight / base.getSprite().getLocalBounds().height));
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

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(50);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition(400, 350); // Posiciona a mensagem na tela

    Hero hero(sf::Vector2f(windowWidth / 2.0f - 25.0f, windowHeight / 2.0f - 25.0f), 200.0f, "heroi.png");

    std::vector<Projectile> projectiles;
    std::vector<Enemy> enemies;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::Clock clock;
    sf::Clock enemySpawnClock; // Relógio para controlar o tempo de spawn dos inimigos
    sf::Clock levelClock;      // Relógio para controlar o tempo para mudar o nível
    sf::Clock gameOverClock;   // Relógio para controlar o tempo após o game over
    sf::Clock levelMessageClock; // Relógio para gerenciar a exibição da mensagem do nível

    bool isGameOver = false;
    bool showLevelMessage = false;
    int currentLevel = 1;

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
            // Aumenta o nível a cada 45 segundos
            if (levelClock.getElapsedTime().asSeconds() >= 45.0f) {
                levelClock.restart();
                ++currentLevel;

                // Atualiza a mensagem do nível
                levelText.setString("Nivel " + std::to_string(currentLevel));
                showLevelMessage = true;
                levelMessageClock.restart(); // Reinicia o relógio para exibir a mensagem
            }

            float minSpawnInterval = 1.0f; // Intervalo mínimo de 1 segundo
            float spawnInterval = std::max(5.0f / currentLevel, minSpawnInterval);
            if (enemySpawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
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

                enemies.emplace_back(sf::Vector2f(xPos, yPos), 50.0f, "inimigos.png", currentLevel); // Corrigido: Adiciona o nível ao criar o inimigo
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
                    it->update(deltaTime, hero.getShape().getPosition(), base.getSprite().getPosition(), projectiles); // Passa a lista de projéteis
                    ++it;
                }
            }

            if (base.getCurrentHealth() > 5) {
                base.regenerate(deltaTime);
            }

            float healthPercentage = base.getCurrentHealth() / 100.0f;
            healthBar.setSize(sf::Vector2f(200.0f * healthPercentage, 20.0f));

            if (base.getCurrentHealth() <= 0.0f) {
                isGameOver = true;
                gameOverClock.restart(); // Inicializa o relógio ao detectar o game over
            }

            // Gerenciar a exibição da mensagem de nível
            if (showLevelMessage && levelMessageClock.getElapsedTime().asSeconds() >= 2.0f) { // Mensagem visível por 2 segundos
                showLevelMessage = false;
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

            // Exibir a mensagem de nível
            if (showLevelMessage) {
                window.draw(levelText);
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
