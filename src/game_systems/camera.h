#pragma once

#include "psyqo/gpu.hh"
#include "psyqo/primitives/common.hh"
#include "psyqo/vector.hh"

class Camera2D
{
public:
    Camera2D(psyqo::Vec2 pos);
public:
    psyqo::Vec2 pos;
    static constexpr psyqo::Vertex CAMERA_OFFSET = {160,120};
    // viewport size
    // zoom
};