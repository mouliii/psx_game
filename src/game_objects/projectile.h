#pragma once

#include "core_systems/graphics.h"
#include "psyqo/vector.hh"

#include "game_objects/character.h"
#include "game_systems/animation.h"
#include "core_systems/primitives/textured_quad.h"

// projectile specific struct

class Projectile : public Character
{
public:
    Projectile() = default;
    ~Projectile() override;
    // copy constructor
    void Setup(Texture* tex, const psyqo::Vec2 pos, const psyqo::Vec2 dir, const psyqo::Vertex size, const Animation anim, const Stats& stats, const int16_t maxDistance, const uint16_t maxPiercing );
    void Update() override;
    void Draw(int layer) override;
    void Attack(Character* character) override;
    void TakeDamage(int16_t amount) override;
private:
    bool alive = true;
    uint16_t nPierced = 0;
    uint16_t maxPiercing = 1;
    int16_t maxDistance;
    psyqo::Vec2 startPos;
    TexturedQuad sprite;
};