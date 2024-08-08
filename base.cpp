#include "base.h"

Base::Base(sf::Vector2f position, float health, float regen)
    : position(position), maxHealth(health), currentHealth(health), regenRate(regen) {
    shape.setSize(sf::Vector2f(300.0f, 200.0f));
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Blue);
}

float Base::getCurrentHealth() const {
    return currentHealth;
}

sf::RectangleShape Base::getShape() const {
    return shape;
}

void Base::takeDamage(float damage) {
    currentHealth -= damage;
    if (currentHealth < 0) {
        currentHealth = 0;
    }
}

void Base::regenerate(float deltaTime) {
    currentHealth += regenRate * deltaTime * 0.3f;
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }
}

void Base::draw(sf::RenderWindow &window) {
    window.draw(shape);
}

bool Base::isDestroyed() {
    return currentHealth <= 0;
}
