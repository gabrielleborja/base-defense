#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "projectile.h"

class Enemy {
private:
    sf::Sprite enemySprite;
    sf::Texture enemyTexture;
    sf::Texture enemyTexture2;
    sf::Vector2f position;
    float speed;
    int health;
    float shootingCooldown;
    float shootingCooldownMax;
    int level; // Nível do inimigo
    float shootRate; // Taxa de disparo em tiros por segundo

public:
    Enemy(sf::Vector2f position, float speed, const std::string& textureFile, int level);

    void update(float deltaTime, const sf::Vector2f& heroPosition, const sf::Vector2f& basePosition, std::vector<Projectile>& projectiles);
    void draw(sf::RenderWindow& window);
    bool isDead();
    sf::Sprite& getSprite();
    void takeDamage();
    bool canShoot();
    void resetShootingCooldown();
    void increaseLevel(int newLevel);
};

#endif // ENEMY_H
