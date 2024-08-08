#include "enemy.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Enemy::Enemy(sf::Vector2f startPosition, float speed)
    : position(startPosition), speed(speed), health(3), shootingCooldown(0.0f), shootingCooldownMax(1.0f) {
    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(position);
}

void Enemy::update(float deltaTime, const sf::RectangleShape& baseShape, std::vector<Projectile>& projectiles) {
    // Move em direção à base
    sf::Vector2f basePos = baseShape.getPosition();
    sf::Vector2f direction = basePos - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    position += direction * speed * deltaTime;
    shape.setPosition(position);

    // Atualiza o cooldown do tiro
    if (shootingCooldown > 0.0f) {
        shootingCooldown -= deltaTime;
    }

    // Verifica se pode atirar
    if (canShoot()) {
        // Atira em direção à base
        projectiles.emplace_back(position, direction, 150.0f, 800.0f, false); // Velocidade do projétil ajustada
        resetShootingCooldown();
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Enemy::isDead() {
    return health <= 0;
}

sf::RectangleShape& Enemy::getShape() {
    return shape;
}

void Enemy::takeDamage() {
    health -= 1;
}

bool Enemy::canShoot() {
    return shootingCooldown <= 0.0f;
}

void Enemy::resetShootingCooldown() {
    shootingCooldown = shootingCooldownMax;
}
