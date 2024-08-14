#include "base.h"

//Construtor
Base::Base(sf::Vector2f position, float health, float regen, const std::string& textureFile) : position(position), maxHealth(health), currentHealth(health), regenRate(regen) {
    if(!baseTexture.loadFromFile("base.png")){
        //error
    }
    baseSprite.setScale(sf::Vector2f(1.0f, 1.0f));
    baseSprite.setTexture(baseTexture);
    baseSprite.setPosition(position);
}

//Getters
float Base::getCurrentHealth() const{
    return currentHealth;
}

sf::Sprite Base::getSprite() const {
    return baseSprite;
}

//Métodos
void Base::takeDamage(float damage) {
    currentHealth -= damage;
    if (currentHealth < 0) {
        currentHealth = 0;
    }
}

//void Base::regenerate(float deltaTime) {
  //  currentHealth += regenRate * deltaTime * 0.3f;
  //if (currentHealth > maxHealth) {
  //  currentHealth = maxHealth;
  //  }
//}

void Base::draw(sf::RenderWindow &window) {
    window.draw(baseSprite);
}

bool Base::isDestroyed() {
    return currentHealth <= 0;
}
