#include "hero.h"
#include <cmath>

Hero::Hero(sf::Vector2f startPosition, float health, int initialAmmo)
    : position(startPosition), speed(200.0f), ammo(initialAmmo), health(health)
{
    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Hero::move(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        position.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        position.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        position.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        position.x += speed * deltaTime;

    shape.setPosition(position);
}

void Hero::shoot(std::vector<Projectile> &projectiles, const sf::Vector2f &target)
{
    if (ammo > 0) {
        sf::Vector2f direction = target - position;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length; // Normalize

        projectiles.emplace_back(position, direction, 300.0f, 800.0f);
        ammo--;
    }
}

void Hero::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

int Hero::getAmmo() const
{
    return ammo;
}

void Hero::addAmmo(int amount)
{
    ammo += amount;
}

void Hero::takeDamage(float damage)
{
    health -= damage;
    if (health < 0)
        health = 0;
}

bool Hero::isDestroyed() const
{
    return health <= 0;
}
