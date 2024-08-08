#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include "projectile.h"

class Hero {
private:
    sf::Vector2f position;
    float speed;
    int ammo;
    float health;
    sf::RectangleShape shape;

public:
    Hero(sf::Vector2f startPosition, float health, int initialAmmo);

    void move(float deltaTime);
    void shoot(std::vector<Projectile> &projectiles, const sf::Vector2f &target);
    void draw(sf::RenderWindow &window);
    int getAmmo() const;
    void addAmmo(int amount);
    void takeDamage(float damage);
    bool isDestroyed() const;
};

#endif
