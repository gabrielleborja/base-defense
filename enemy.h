#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "projectile.h"

class Enemy {
private:
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    float health;
    sf::RectangleShape shape; // Atualizado para RectangleShape
    sf::Clock attackClock;
    float attackCooldown;

public:
    // Construtor
    Enemy(sf::Vector2f startPosition, sf::Vector2f moveDirection, float moveSpeed, float enemyHealth, float cooldown);

    // Getters
    sf::RectangleShape getShape() const;

    // Métodos
    void update(float deltaTime, std::vector<Projectile>& projectiles, const sf::Vector2f& basePosition);
    bool isDestroyed() const;
    void takeDamage(float damage);
    void draw(sf::RenderWindow &window) const;
    bool checkCollision(const sf::RectangleShape &target) const;
};

#endif
