#include "hero.h"
#include <math.h>
#include <iostream>

Hero::Hero(sf::Vector2f position, float speed, const std::string& textureFile)
    : speed(speed), direction(0.0f, 0.0f), shootingCooldown(0.0f), shootingCooldownMax(0.5f), ammo(100) {
    if(!heroTexture.loadFromFile("assets/images/heroi.png")){
        std::cerr << "Erro ao carregar a textura heroi.png" << std::endl;
    }
    heroSprite.setTexture(heroTexture);
    heroSprite.setScale(sf::Vector2f(1.0f, 1.0f)); // Define o tamanho do herói
    heroSprite.setPosition(position);
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
    heroSprite.move(direction * speed * deltaTime);

    // Atualiza o cooldown do tiro
    if (shootingCooldown > 0.0f) {
        shootingCooldown -= deltaTime;
    }
}

void Hero::draw(sf::RenderWindow& window) {
    window.draw(heroSprite);
}

sf::Sprite& Hero::getShape() {
    return heroSprite;
}

sf::Vector2f Hero::getDirection() {
    return direction;
}

bool Hero::hasAmmo() const {
    return ammo > 0;
}

void Hero::useAmmo() {
    if (ammo > 0) {
        ammo--;
    }
}

void Hero::addAmmo() {
    if (ammo < 100) {
        ammo = ammo + 2;
    }
}

int Hero::getAmmo() const {
    return ammo;
}