#include "enemy.h"
#include <cmath>

Enemy::Enemy(sf::Vector2f startPosition, sf::Vector2f moveDirection, float moveSpeed, float enemyHealth, float cooldown)
    : position(startPosition), direction(moveDirection), speed(moveSpeed), health(enemyHealth), attackCooldown(cooldown) {
    shape.setSize(sf::Vector2f(40.0f, 40.0f)); // Ajuste do tamanho para retângulo
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
    attackClock.restart();
}

sf::RectangleShape Enemy::getShape() const {
    return shape;
}

void Enemy::update(float deltaTime, std::vector<Projectile>& projectiles, const sf::Vector2f& basePosition) {
    sf::Vector2f moveDirection = basePosition - position;
    float length = std::sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
    if (length != 0) {
        moveDirection /= length;
    }
    position += moveDirection * speed * deltaTime;
    shape.setPosition(position);

    if (attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
        sf::Vector2f projectileDirection = moveDirection;
        float projectileSpeed = 200.0f;
        float projectileRange = 400.0f;
        projectiles.emplace_back(position + moveDirection * shape.getSize().x / 2.0f, projectileDirection, projectileSpeed, projectileRange);
        attackClock.restart();
    }
}

bool Enemy::isDestroyed() const {
    return health <= 0;
}

void Enemy::takeDamage(float damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Enemy::draw(sf::RenderWindow &window) const {
    window.draw(shape);
}

bool Enemy::checkCollision(const sf::RectangleShape &target) const {
    return shape.getGlobalBounds().intersects(target.getGlobalBounds());
}
