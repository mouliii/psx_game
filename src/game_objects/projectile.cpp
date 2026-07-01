#include "projectile.h"

Projectile::~Projectile()
{
}

void Projectile::Setup(Texture *tex, const psyqo::Vec2 pos, const psyqo::Vec2 dir, const psyqo::Vertex size, const Animation anim, const Stats &stats, const int16_t maxDistance, const uint16_t maxPiercing)
{
    Initialize(tex, pos, size, anim, stats);
    this->maxDistance = maxDistance;
    this->maxPiercing = maxPiercing;
    this->startPos = pos;
    sprite.texture = tex;
    sprite.size = {16,16};
    sprite.uv = {{8*16,0},{16,16}};
}

void Projectile::Update()
{
    pos += dir * stats.speed;
    psyqo::Vec2 dPos = pos - startPos;
    int16_t curDistance = (dPos.x.integer() * dPos.x.integer()) + (dPos.y.integer() * dPos.y.integer());
    if (curDistance > (maxDistance * maxDistance))
    {
        alive = false;
    }
    sprite.position = pos;
    anim.Update();
}

void Projectile::Draw(int layer)
{
    //psyqo::Rect rect = anim.GetFrame();  
    //Graphics::Instance().DrawTexturedQuad(tex, pos, size, rect, layer, color);
    Graphics::Instance().DrawTexturedQuad(sprite, layer);
}

void Projectile::Attack(Character *character)
{
    character->TakeDamage(stats.damage);
    if (nPierced > maxPiercing)
    {
        alive = false;
    }
    nPierced++;
}

void Projectile::TakeDamage(int16_t amount)
{
}
