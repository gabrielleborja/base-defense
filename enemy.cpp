#include "enemy.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

Enemy::Enemy(sf::Vector2f startPosition, float speed, const std::string& textureFile)
    : position(startPosition), speed(speed), health(3), shootingCooldown(0.0f), shootingCooldownMax(1.0f) {
    if(!enemyTexture.loadFromFile("inimigos.png")){
        std::cerr << "Erro ao carregar a textura inimigos.png" << std::endl;
    }
    enemySprite.setTexture(enemyTexture);
    enemySprite.setScale(sf::Vector2f(0.5f, 0.5f));
    enemySprite.setPosition(position);
}

void Enemy::update(float deltaTime, const sf::Vector2f& heroPosition, const sf::Vector2f& basePosition, std::vector<Projectile>& projectiles) {
    // Movimenta-se em direção ao herói
    sf::Vector2f directionToHero = heroPosition - position;
    float lengthToHero = std::sqrt(directionToHero.x * directionToHero.x + directionToHero.y * directionToHero.y);
    if (lengthToHero != 0) {
        directionToHero /= lengthToHero; // Normaliza a direção
    }

    position += directionToHero * speed * deltaTime;
    enemySprite.setPosition(position);

    // Atualiza o cooldown do tiro
    if (shootingCooldown > 0.0f) {
        shootingCooldown -= deltaTime;
    }

    // Verifica se pode atirar
    if (canShoot()) {
        // Calcula a direção do tiro em direção à base
        sf::Vector2f directionToBase = basePosition - position;
        float lengthToBase = std::sqrt(directionToBase.x * directionToBase.x + directionToBase.y * directionToBase.y);
        if (lengthToBase != 0) {
            directionToBase /= lengthToBase; // Normaliza a direção
        }

        // Atira em direção à base
        projectiles.emplace_back(position, directionToBase, 150.0f, 800.0f, false);
        resetShootingCooldown();
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(enemySprite);
}

bool Enemy::isDead() {
    return health <= 0;
}

sf::Sprite& Enemy::getSprite() {
    return enemySprite;
}

void Enemy::takeDamage() {
    health -= 1;
}

bool Enemy::canShoot() {
    return shootingCooldown <= 0.0f;
}

void Enemy::resetShootingCooldown() {
    shootingCooldown = shootingCooldownMax;
}
