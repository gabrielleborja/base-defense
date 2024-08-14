#include "projectile.h"
#include <cmath>

// Construtor atualizado
Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f direction, float speed, float range, bool isFromHero)
    : position(startPosition), direction(direction), speed(speed), maxRange(range), traveledDistance(0.0f), heroProjectile(isFromHero) {
    shape.setRadius(5.0f);
    shape.setPosition(position);
    shape.setFillColor(isFromHero ? sf::Color::Red : sf::Color::Red); // Azul para projéteis do herói
}

// Getters
sf::Vector2f Projectile::getPosition() const {
    return position;
}

sf::CircleShape Projectile::getShape() const {
    return shape;
}

bool Projectile::isHero() const {
    return heroProjectile;
}

// Métodos
void Projectile::update(float deltaTime) {
    sf::Vector2f velocity = direction * speed * deltaTime;
    position += velocity;
    traveledDistance += std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    shape.setPosition(position);
}

bool Projectile::hasReachedMaxRange() {
    return traveledDistance >= maxRange;
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Projectile::checkCollision(const sf::Sprite& target) {
    return shape.getGlobalBounds().intersects(target.getGlobalBounds());
}
