#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile {
public:
    // Construtor atualizado
    Projectile(sf::Vector2f startPosition, sf::Vector2f direction, float speed, float range, bool isFromHero);

    // Getters
    sf::Vector2f getPosition() const;
    sf::CircleShape getShape() const;
    bool isHero() const;

    // Métodos
    void update(float deltaTime);
    bool hasReachedMaxRange();
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::Sprite& target);

private:
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    float maxRange;
    float traveledDistance;
    bool heroProjectile;  // Novo membro que indica se o projétil é do herói

    sf::CircleShape shape;
};

#endif // PROJECTILE_H
