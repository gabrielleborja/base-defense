#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "projectile.h"

class Enemy {
private:
    sf::RectangleShape shape;
    sf::Vector2f position;
    float speed;
    int health;
    float shootingCooldown;
    float shootingCooldownMax;

public:
    Enemy(sf::Vector2f position, float speed);

    void update(float deltaTime, const sf::RectangleShape& baseShape, std::vector<Projectile>& projectiles);
    void draw(sf::RenderWindow& window);
    bool isDead();
    sf::RectangleShape& getShape();
    void takeDamage();
    bool canShoot();
    void resetShootingCooldown();
};

#endif // ENEMY_H
