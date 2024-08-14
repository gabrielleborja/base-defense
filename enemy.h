#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "projectile.h"

class Enemy {
private:
    sf::Sprite enemySprite;
    sf::Texture enemyTexture;
    sf::Vector2f position;
    float speed;
    int health;
    float shootingCooldown;
    float shootingCooldownMax;

public:
    Enemy(sf::Vector2f position, float speed, const std::string& textureFile);

    void update(float deltaTime, const sf::Sprite& baseShape, std::vector<Projectile>& projectiles);
    void draw(sf::RenderWindow& window);
    bool isDead();
    sf::Sprite& getSprite();
    void takeDamage();
    bool canShoot();
    void resetShootingCooldown();
};

#endif // ENEMY_H
