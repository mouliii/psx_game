#pragma once

#include "EASTL/memory.h"
#include "psyqo/gpu.hh"
#include "psyqo/primitives.hh"
#include "psyqo/fragments.hh"
#include "psyqo/ordering-table.hh"
#include "psyqo/bump-allocator.hh"
#include "psyqo/advancedpad.hh"
#include "psyqo/primitives/common.hh"
#include "psyqo/soft-math.hh"

#include "../core_systems/asset_manager.h"
#include "../core_systems/graphics.h"
#include "../game_systems/animation.h"

struct Stats
{
    int16_t health;
    int16_t damage;
    psyqo::FixedPoint<> speed;
    int16_t cooldownReduction = 0;
};

class Character
{
public:
    Character() = default;
    virtual ~Character();
    void Initialize(Texture* tex, const psyqo::Vec2 pos, const psyqo::Vertex size, const Animation anim, const Stats& stats );
    virtual void Update(psyqo::AdvancedPad& pad);
    virtual void Draw(Graphics& gfx, int layer = 3) = 0;
    // TODO: projectile
    virtual void Attack(Character* character) = 0;
    virtual void TakeDamage(int16_t amount) = 0;
    void SetPosition(psyqo::Vec2 newPos){pos = newPos;};
    void SetSpeed(psyqo::FixedPoint<> newSpeed){stats.speed = newSpeed;};
    void SetDamage(int16_t dmg){stats.damage = dmg;};
    void SetHealth(int16_t health){stats.health = health;};
    void SetAnimation(const Animation& anim){this->anim = anim;};
public:
    psyqo::Vec2 pos;
    psyqo::Vertex size;
    psyqo::Vec2 dir = {0,0};
    int8_t lastFacing = 0;
    Texture* tex;
    psyqo::Color color{128,128,128};
    Animation anim;
    Stats stats;
    int16_t attackCooldown;
    int16_t currentCooldown;
private:

};