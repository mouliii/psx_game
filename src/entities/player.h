#pragma once

#include "EASTL/memory.h"
#include "psyqo/gpu.hh"
#include "psyqo/primitives.hh"
#include "psyqo/fragments.hh"
#include "psyqo/ordering-table.hh"
#include "psyqo/bump-allocator.hh"
#include "psyqo/advancedpad.hh"
#include "psyqo/primitives/common.hh"
#include "../core_systems/asset_manager.h"
#include "../core_systems/graphics.h"
#include "../game_systems/animation.h"

class Player
{
public:
    Player(Texture* tex, psyqo::Vec2 pos, psyqo::Vertex size);
    ~Player();
    void Update(psyqo::AdvancedPad& pad);
    void Draw(Graphics& gfx, int layer = 3);
public:
    psyqo::Vec2 pos;
    psyqo::Vertex size;
    uint16_t speed = 3;
    int dir = 0;
    // pos = vel(speed) * dir
    Texture* tex;
    psyqo::Color color;
    Animation anim;
private:

};