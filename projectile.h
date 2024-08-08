#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile
{
private:
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    float maxRange;
    float traveledDistance;
    sf::CircleShape shape;

public:
    // Construtor
    Projectile(sf::Vector2f startPosition, sf::Vector2f direction, float speed, float range);

    // Getters
    sf::Vector2f getPosition() const;
    sf::CircleShape getShape() const;

    // Métodos da classe
    void update(float deltaTime);
    bool hasReachedMaxRange();
    void draw(sf::RenderWindow &window);
    bool checkCollision(const sf::Shape &target); // Atualizado para aceitar sf::Shape
};

#endif
