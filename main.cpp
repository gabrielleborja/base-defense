#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

    sf::SoundBuffer gameOverBuffer;
    if (!gameOverBuffer.loadFromFile("endSound.wav")) {
        std::cerr << "Erro ao carregar o som de disparo!" << std::endl;
        return -1;
    }
    sf::Sound overSound;
    overSound.setBuffer(gameOverBuffer);

    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("bgMusic.wav")) {
        std::cerr << "Erro ao carregar a música de fundo!" << std::endl;
        return -1;
    }
    backgroundMusic.setLoop(true); 
    backgroundMusic.play();         

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("bg.png")) {
        std::cerr << "Erro ao carregar a textura de background!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        windowWidth / backgroundTexture.getSize().x + 0.5f,
        windowHeight / backgroundTexture.getSize().y + 0.5f
    );

   sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("gameover.png")) {
        std::cerr << "Erro ao carregar a textura de background!" << std::endl;
        return -1;
    }
    sf::Sprite gameOverSprite;
    gameOverSprite.setTexture(gameOverTexture);
    gameOverSprite.setScale(
        windowWidth / gameOverTexture.getSize().x,
        windowHeight / gameOverTexture.getSize().y
    );

    sf::Vector2f basePosition(
        ((windowWidth - baseWidth)/ 2.0f) +220.0f,
        ((windowHeight - baseHeight)/ 2.0f) +55.0f
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

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(70);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition(500, 350); // Posiciona a mensagem na tela

    sf::Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(20);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(1050, 50);

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
        ammoText.setString("Munição: " + std::to_string(hero.getAmmo()));
        window.draw(ammoText);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!isGameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (hero.hasAmmo()) {
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
                    hero.useAmmo();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();


        if (!isGameOver) {
            ammoText.setString("Ammo: " + std::to_string(hero.getAmmo()));
        }

        if (!isGameOver) {
            // Aumenta o nível a cada 30 segundos
            if (levelClock.getElapsedTime().asSeconds() >= 30.0f) {
                levelClock.restart();
                ++currentLevel;
                for(int i=0; i<currentLevel; i++){
                    hero.addAmmo();
                }

                levelText.setString("Nivel " + std::to_string(currentLevel));
                showLevelMessage = true;
                levelMessageClock.restart(); 
            }

            float minSpawnInterval = 1.0f;
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
                        base.takeDamage(3.0f);  // Causa 3 de dano à base
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
                    hero.addAmmo();
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
                gameOverClock.restart(); 
                overSound.play();
            }

            // Gerenciar a exibição da mensagem de nível
            if (showLevelMessage && levelMessageClock.getElapsedTime().asSeconds() >= 2.0f) { 
                showLevelMessage = false;
            }
        }

        window.clear();

        if (!isGameOver) {
            window.draw(backgroundSprite);
            base.draw(window);
            window.draw(healthBarBackground);
            window.draw(healthBar);

            hero.handleInput();
            hero.update(deltaTime);
            hero.draw(window);

            if (showLevelMessage) {
                window.draw(levelText);
            }

            for (auto& projectile : projectiles) {
                projectile.draw(window);
            }

            for (auto& enemy : enemies) {
                enemy.draw(window);
            }

            window.draw(ammoText);
        } 
        else {
            window.draw(gameOverSprite);

            // Fechar a janela após 4 segundos de exibição da mensagem de Game Over
            if (gameOverClock.getElapsedTime().asSeconds() >= 4.0f) {
                window.close();
            }
        }

        window.display();
    }

    return 0;
}
