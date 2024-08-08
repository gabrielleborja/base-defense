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
        //Construtor
        Base(sf::Vector2f position, float health, float regen);

        //Getters
        float getCurrentHealth() const;
        sf::RectangleShape getShape() const;

        //Métodos
        void takeDamage(float damage);
        void regenerate(float deltaTime);
        void draw(sf::RenderWindow &window);
        bool isDestroyed();
};

#endif