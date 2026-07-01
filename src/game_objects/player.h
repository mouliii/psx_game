#pragma once


#include "game_objects/character.h"
#include "core_systems/gamepad.h"
#include "game_objects/projectile.h"
#include "core_systems/primitives/textured_quad.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;
    void Update() override;
    void Draw(int layer) override;
    void HandleDigitalPad();
    void HandleAnalogPad();
    void SetupProjectile(Projectile projectile);
    void Attack(Character* character) override;
    void ShootProjectile();
    void TakeDamage(int16_t amount) override;
private:
    psyqo::Vec2 aimDir;
    bool shooting = false;
    Projectile projectile;
    TexturedQuad reticle;
};