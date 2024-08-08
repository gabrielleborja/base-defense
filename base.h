#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>

class Base {
private:
    sf::Vector2f position;
    float maxHealth;
    float currentHealth;
    float regenRate;
    sf::RectangleShape shape;

public:
    Base(sf::Vector2f position, float health, float regen);

    float getCurrentHealth() const;
    sf::RectangleShape getShape() const;

    void takeDamage(float damage);
    void regenerate(float deltaTime);
    void draw(sf::RenderWindow &window);
    bool isDestroyed();
};

#endif
