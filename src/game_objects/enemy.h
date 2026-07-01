#pragma once

#include "EASTL/memory.h"
#include "psyqo/gpu.hh"
#include "psyqo/primitives.hh"
#include "psyqo/fragments.hh"
#include "psyqo/ordering-table.hh"
#include "psyqo/bump-allocator.hh"
#include "psyqo/primitives/common.hh"
#include "psyqo/soft-math.hh"

#include "core_systems/asset_manager.h"
#include "core_systems/graphics.h"
#include "game_systems/animation.h"
#include "game_objects/character.h"

class Enemy : public Character
{
public:
    Enemy();
    ~Enemy();
    void Update(const psyqo::Vec2& target);
    void Draw(int layer) override;
    // TODO: projectile
    void Attack(Character* character) override;
    void TakeDamage(int16_t amount) override;
private:
    int16_t threshold = 16;
};