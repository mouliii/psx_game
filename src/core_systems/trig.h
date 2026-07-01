#pragma once

#include "psyqo/trigonometry.hh"
#include "psyqo/vector.hh"

namespace mouli
{
extern psyqo::Trig<> trig;
psyqo::Vec2 RotatePoint(const psyqo::Vec2 point, const psyqo::Vec2 pivot, psyqo::Angle angle);
}
