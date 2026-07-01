#include "trig.h"
#include "psyqo/vector.hh"

namespace mouli
{
psyqo::Trig<> trig;
psyqo::Vec2 RotatePoint(const psyqo::Vec2 point, const psyqo::Vec2 pivot, psyqo::Angle angle)
{
    psyqo::Vertex deltaPos = point - pivot;

    auto c = mouli::trig.cos(angle);
    auto s = mouli::trig.sin(angle);

    return {
        c * deltaPos.x - s * deltaPos.y + pivot.x,
        s * deltaPos.x + c * deltaPos.y + pivot.y
    };
}
}