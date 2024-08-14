#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>

class Base {
    private:
        sf::Vector2f position;
        float maxHealth;
        float currentHealth;
        float regenRate;
        sf::Sprite baseSprite;
        sf::Texture baseTexture;

    public:
        //Construtor
        Base(sf::Vector2f position, float health, float regen, const std::string& textureFile);

        //Getters
        float getCurrentHealth() const;
        sf::Sprite getSprite() const;

        //Métodos
        void takeDamage(float damage);
        void regenerate(float deltaTime);
        void draw(sf::RenderWindow &window);
        bool isDestroyed();
};

#endif