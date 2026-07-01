#pragma once

#include "psyqo/vector.hh"
#include "core_systems/asset_manager.h"
#include "core_systems/trig.h"

struct TexturedQuad
{
    psyqo::Vec2 position;
    psyqo::Vertex size;
    psyqo::Angle angle = 0;
    Texture* texture;
    psyqo::Rect uv;
    psyqo::Color color = {128,128,128};
};