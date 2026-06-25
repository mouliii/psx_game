#include "enemy.h"

Enemy::Enemy()
{}

Enemy::~Enemy()
{}

void Enemy::Update(const psyqo::Vec2& target)
{
    dir = {0,0};
    const psyqo::Vec2 deltaPos = target - pos;
    const int32_t distSq = deltaPos.x.integer() * deltaPos.x.integer() + deltaPos.y.integer() * deltaPos.y.integer();

    if (distSq > (threshold * threshold))
    {
        psyqo::Vec3 dirVecNorm{deltaPos.x, deltaPos.y, 0};
        psyqo::SoftMath::normalizeVec3(&dirVecNorm);
        dir = {dirVecNorm.x, dirVecNorm.y};
        pos += dir * stats.speed;
    }
    else
    {
        // melee attack
    }

    if (dir.x != 0){
        anim.ResumeAnimation();
        lastFacing = dir.x.integer();
    }else{
        anim.PauseAnimation();
    }

    anim.Update();
}

void Enemy::Draw(Graphics &gfx, int layer)
{
    psyqo::Rect rect = anim.GetFrame();
    if (lastFacing < 0)
    {
        anim.InvertX(rect);
    }
    
    gfx.DrawTexturedQuad(tex, pos, size, rect, layer, color);
}

void Enemy::Attack(Character* character)
{
    // melee
    character->TakeDamage(stats.damage);
    // ranged
}

void Enemy::TakeDamage(int16_t amount)
{
    stats.health -= amount;
}
