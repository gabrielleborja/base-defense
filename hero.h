#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include "projectile.h"

class Hero {
    
private:
    sf::Sprite heroSprite;
    sf::Texture heroTexture;
    sf::Vector2f direction;
    float speed;
    float shootingCooldown;
    float shootingCooldownMax;
    int ammo;
public:
    Hero(sf::Vector2f position, float speed, const std::string& textureFile);
    void handleInput();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Sprite& getShape();
    sf::Vector2f getDirection();
    bool hasAmmo() const; 
    void useAmmo(); 
    int getAmmo() const; 
    void addAmmo();

};

#endif // HERO_H
