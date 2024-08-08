#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include "projectile.h"

class Hero {
public:
    Hero(sf::Vector2f position, float speed);

    void handleInput();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::RectangleShape& getShape();
    sf::Vector2f getDirection();

private:
    sf::RectangleShape shape;
    sf::Vector2f direction;
    float speed;
    float shootingCooldown;
    float shootingCooldownMax;
};

#endif // HERO_H
