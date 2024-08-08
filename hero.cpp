#include "hero.h"
#include <math.h>

Hero::Hero(sf::Vector2f position, float speed)
    : speed(speed), direction(0.0f, 0.0f), shootingCooldown(0.0f), shootingCooldownMax(0.5f) {
    shape.setSize(sf::Vector2f(50.0f, 50.0f)); // Define o tamanho do herói
    shape.setFillColor(sf::Color::Green); // Define a cor do herói
    shape.setPosition(position);
}

void Hero::handleInput() {
    direction = sf::Vector2f(0.0f, 0.0f); // Reinicia a direção

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x += 1.0f;
    }

    // Normaliza a direção se não for zero
    if (direction != sf::Vector2f(0.0f, 0.0f)) {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
    }
}

void Hero::update(float deltaTime) {
    shape.move(direction * speed * deltaTime);

    // Atualiza o cooldown do tiro
    if (shootingCooldown > 0.0f) {
        shootingCooldown -= deltaTime;
    }
}

void Hero::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::RectangleShape& Hero::getShape() {
    return shape;
}

sf::Vector2f Hero::getDirection() {
    return direction;
}
